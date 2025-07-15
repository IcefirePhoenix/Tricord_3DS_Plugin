#include "Helpers.hpp"
#include "Cheats.hpp"

#define MAX_SPLITS 4
namespace CTRPluginFramework
{
    Clock speedTimer;
    Clock autoSplitCooldown;
    Time pauseStartTime, pauseStartTimeRelative, pauseEndTime, accumulatedPauseDuration = Time::Zero;

    Clock splitTimer;
    std::deque<Time> splits;

    int xCoord = 5, yCoord = 225;

    bool running = true;
    bool alwaysShowSplits = false;

    bool autoSplit = false;
    bool autoTimerEvents[6] = {};
    bool autoRestart = false;
    int pauseEventID = -1;

    const StringVector timerEvents =
    {
        "Pause timer during cutscenes",
        "Pause timer during loading screens",
        "Pause timer in treasure rooms",
        "Pause timer while game is paused",
        "Restart timer upon entering a new level",
        "Auto-split upon entering a new area",
        "Run timer during cutscenes",
        "Run timer during loading screens",
        "Run timer in treasure rooms",
        "Run timer while game is paused",
        "Continue timer upon entering a new level",
        "Don't auto-split when entering a new area"
    };

    // Stop updating time and store time at which timer was paused
    void pause(int eventID)
    {
        running = false;
        pauseStartTime = speedTimer.GetElapsedTime();
        pauseStartTimeRelative = pauseStartTime - accumulatedPauseDuration;
        pauseEventID = eventID;
    }

    // Add to accumulated pause duration and continue
    void resume(void)
    {
        pauseEndTime = speedTimer.GetElapsedTime();
        accumulatedPauseDuration += (pauseEndTime - pauseStartTime);
        running = true;
    }

    void reset(void)
    {
        pauseStartTime = Time::Zero;
        pauseStartTimeRelative = Time::Zero;
        accumulatedPauseDuration = Time::Zero;
        autoRestart = false;
        speedTimer.Restart();
        splits.clear();
    }

    void displayTime(Time time, int x, int y)
    {
        int secondsRaw = (int)time.AsSeconds();
        int hours = secondsRaw / 3600;
        int minutes = (secondsRaw - hours*60) / 60;
        int seconds = secondsRaw - hours*3600 - minutes*60;
        int milliseconds = time.AsMilliseconds() - hours*3600000 - minutes*60000 - seconds*1000;
        char timeStr[13];

        sprintf(timeStr, "%02d:%02d:%02d.%03d", hours, minutes, seconds, milliseconds);
        Renderer::SetTarget(TOP);
        Renderer::DrawString(timeStr, x, y, Color::White, Color::Black);
    }

    void displaySplits(void)
    {
        if (splits.empty())
            return;

        if (!splitTimer.HasTimePassed(Seconds(10)) || alwaysShowSplits)
        {
            for (int rendered = 0; rendered < splits.size(); rendered++)
            {
                // Each split is positioned 15 pixels apart
                int yDist = 15 * (rendered + 1);
                displayTime(splits[splits.size() - rendered - 1], xCoord, yCoord - yDist);
            }
        }
    }

    void Miscellaneous::speedrunTimer(MenuEntry* entry)
    {
        // Init cooldown timer
        if (entry->WasJustActivated())
            autoSplitCooldown.Restart();

        // Restart conditions
        if (GeneralHelpers::isLoadingScreen(true) && autoSplitCooldown.HasTimePassed(Seconds(2.0)))
        {
            u8 targetLevel = Level::getTargetLevel();
            u8 targetStage = Level::getTargetStage();

            if (targetLevel == 0xFF || targetStage == 0xFF)
            {
                OSD::Notify("[ERROR] Speedrun timer cannot determine current location data.", Color::Red);
                return;
            }
            else
            {
                // written a bit strangely; this logic accommodates the usage of autoRestart for autoSplit
                autoRestart = Level::getCurrLevel() != targetLevel;

                if (autoTimerEvents[5])
                    autoSplit = (Level::getCurrStage() != targetStage) || autoRestart;

                if (!autoTimerEvents[4])
                    autoRestart = false;

                // Used to prevent multiple overwrites during loading screen checks...
                autoSplitCooldown.Restart();
            }
        }

        // Reset conditions
        if (entry->WasJustActivated() || entry->Hotkeys[1].IsPressed() || autoRestart)
            reset();

        // Create and show splits
        if (entry->Hotkeys[0].IsPressed() || autoSplit)
        {
            Time newSplit = running ? speedTimer.GetElapsedTime() - accumulatedPauseDuration : pauseStartTimeRelative;

            if (splits.size() >= MAX_SPLITS)
                splits.pop_front();

            if (newSplit != Time::Zero)
                splits.push_back(newSplit);

            autoSplit = false;
            splitTimer.Restart();
        }
        displaySplits();

        // Manage pause events
        if (running)
        {
            if (autoTimerEvents[0] && (Freecam::getCameraType() > CameraMode::DYNAMIC))
            {
                pause(0);
                return;
            }
            if (autoTimerEvents[3] && GeneralHelpers::isPauseScreen())
            {
                pause(3);
                return;
            }
            if (autoTimerEvents[2] && (GeneralHelpers::isLoadingScreen(true) && Level::getTargetStage() == 5))
            {
                pause(2);
                return;
            }
            if (autoTimerEvents[1] && GeneralHelpers::isLoadingScreen(true))
            {
                pause(1);
                return;
            }

            displayTime(speedTimer.GetElapsedTime() - accumulatedPauseDuration, xCoord, yCoord);
        }
        else
        {
            // Draw pause-time on screen
            displayTime(pauseStartTimeRelative, xCoord, yCoord);

            // Wait for appropriate unpause status
            switch (pauseEventID)
            {
                case 0:
                    if (Freecam::getCameraType() <= CameraMode::DYNAMIC)
                        resume();
                    break;
                case 1:
                    if (Level::getElapsedTime() == (Level::getCurrLevel() < 4 ? 30 : 100) && !(autoTimerEvents[2] && Level::getCurrStage() == 5))
                        resume();
                    break;
                case 2:
                    if (GeneralHelpers::isLoadingScreen(true) && Level::getTargetStage() != 5)
                    {
                        do reset();
                        while (speedTimer.GetElapsedTime().AsMilliseconds() != 0);

                        resume();
                    }
                    break;
                case 3:
                    if (!GeneralHelpers::isPauseScreen())
                        resume();
                    break;
                default:
                    // Something unexpected happened, just resume as a failsafe
                    resume();
                    break;
            }
        }
    }

    // Toggle various automatic triggers to pause/restart the timer
    void Miscellaneous::toggleTimerEvents(MenuEntry *entry)
    {
        int arg = reinterpret_cast<int>(entry->GetArg());
        if (entry->Name() == timerEvents[arg])
        {
            autoTimerEvents[arg] = true;
            entry->SetName(timerEvents[arg + 6]);
        }
        else
        {
            autoTimerEvents[arg] = false;
            entry->SetName(timerEvents[arg]);
        }
    }

    // Toggles display status of splits
    void Miscellaneous::toggleSplits(MenuEntry *entry)
    {
        alwaysShowSplits = !alwaysShowSplits;

        if (alwaysShowSplits)
            entry->SetName("Auto-hide splits after 10 seconds");
        else
            entry->SetName("Always show splits on-screen");
    }
}