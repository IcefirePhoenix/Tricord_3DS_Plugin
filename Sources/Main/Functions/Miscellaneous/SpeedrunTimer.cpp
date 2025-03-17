#include "Helpers.hpp"
#include "Cheats.hpp"

namespace CTRPluginFramework
{
    Clock speedTimer;
    Time pauseStartTime, pauseStartTimeRelative, pauseEndTime, accumulatedPauseDuration = Time::Zero;

    Clock splitTimer;
    Time split;

    Screen screen = OSD::GetTopScreen();
    int xCoord = 5, yCoord = 225;

    bool running = true, showSplit = false;
    bool autoTimerEvents[5] = {false, false, false, false, false};
    bool autoRestart = false;
    int pauseEventID = -1;
    const StringVector timerEvents =
    {
        "Pause timer during cutscenes",
        "Pause timer during loading screens",
        "Pause timer in treasure rooms",
        "Pause timer while game is paused",
        "Restart timer upon entering a new level",
        "Run timer during cutscenes",
        "Run timer during loading screens",
        "Run timer in treasure rooms",
        "Run timer while game is paused",
        "Continue timer upon entering a new level"
    };

    u8 prevLvl = 0;

    // Implement pause and resume functions

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

    void displayTime(Time time, int x, int y)
    {
        int secondsRaw = (int)time.AsSeconds();
        int hours = secondsRaw / 3600;
        int minutes = (secondsRaw - hours*60) / 60;
        int seconds = secondsRaw - hours*3600 - minutes*60;
        int milliseconds = time.AsMilliseconds() - hours*3600000 - minutes*60000 - seconds*1000;
        char timeStr[13];
        sprintf(timeStr, "%02d:%02d:%02d.%03d", hours, minutes, seconds, milliseconds);
        screen.Draw(timeStr, x, y, Color::White, Color(0,0,0,0));
    }

    void Miscellaneous::speedrunTimer(MenuEntry* entry)
    {
        // Restart conditions
        if (Level::getElapsedTime() == 105)
            prevLvl = Level::getCurrLevel(); // Always store level as "previous" after each loading zone

        if (autoTimerEvents[4] && GeneralHelpers::isLoadingScreen(false))
            autoRestart = Level::getCurrLevel() != prevLvl; // Check current (new) level against stored "previous" level

        if (entry->WasJustActivated() || entry->Hotkeys[1].IsPressed() || (autoRestart && Level::getElapsedTime() == (Level::getCurrLevel() < 4 ? 31 : 101)))
        {
            speedTimer.Restart();
            accumulatedPauseDuration = Time::Zero;
            autoRestart = false;
        }

        // Create and show split
        if (entry->Hotkeys[0].IsPressed())
        {
            split = running ? speedTimer.GetElapsedTime() - accumulatedPauseDuration : pauseStartTimeRelative;
            showSplit = true;
            splitTimer.Restart();
        }

        if (showSplit)
        {
            // Display 15 pixels above usual timer
            displayTime(split, xCoord, yCoord - 15);
            if (splitTimer.HasTimePassed(Seconds(10)))
                showSplit = false;
        }
        
        
        if (running)
        {
            // Check for pause events
            if (autoTimerEvents[0])
            {
                u8 camMode;
                Process::Read8(AddressList::getAddress("CameraMode"), camMode);
                if (camMode > 1)
                {
                    pause(0);
                    return;
                }
            }
            if (autoTimerEvents[1])
            {
                if (GeneralHelpers::isLoadingScreen(true))
                {
                    pause(1);
                    return;
                }
            }
            if (autoTimerEvents[2])
            {
                if (Level::getCurrStage() == 5)
                {
                    pause(2);
                    return;
                }
            }
            if (autoTimerEvents[3])
            {
                if (GeneralHelpers::isPauseScreen())
                {
                    pause(3);
                    return;
                }
            }

            // Run and display time

            // Draw
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
                    u8 camMode;
                    Process::Read8(AddressList::getAddress("CameraMode"), camMode);
                    if (camMode <= 1)
                        resume();
                    break;
                case 1:
                    if (Level::getElapsedTime() == (Level::getCurrLevel() < 4 ? 30 : 100))
                        resume();
                    break;
                case 2:
                    if (Level::getCurrStage() != 5)
                        resume();
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
    void Miscellaneous::toggleTimerEvents(MenuEntry* entry)
    {
        int arg = reinterpret_cast<int>(entry->GetArg());
        if (entry->Name() == timerEvents[arg])
        {
            autoTimerEvents[arg] = true;
            entry->SetName(timerEvents[arg + 5]);
        }
        else
        {
            autoTimerEvents[arg] = false;
            entry->SetName(timerEvents[arg]);
        }
    }
}