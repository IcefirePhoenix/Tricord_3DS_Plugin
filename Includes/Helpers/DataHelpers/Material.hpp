#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "CTRPluginFramework.hpp"

namespace CTRPluginFramework {
    class Material {
    public:
        static const StringVector otherMats;
        static const StringVector woodlandMats;
        static const StringVector riversideMats;
        static const StringVector volcanoMats;
        static const StringVector iceMats;
        static const StringVector fortressMats;
        static const StringVector dunesMats;
        static const StringVector ruinsMats;
        static const StringVector skyMats;

        static int selectMaterialIndiv(int selectedWorld);
        static int selectMaterialWorld(void);
        static std::string getMaterialName(int world, int mat);
    };
}
#endif

