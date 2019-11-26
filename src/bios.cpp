#include "bios.h"
#include "vectrex.h"

namespace bios {

    void Init() {
        SetBeamIntensity(DefaultIntensity);
        SetScale(DefaultScale);
    }

    void WaitFrame() {
        Wait_Recal();

        // Need to set beam intensity after Wait_Recal
        SetBeamIntensity(DefaultIntensity);

        // SetScale(DefaultScale);
    }

    void ZeroBeam() { Reset0Ref(); }

    void SetBeamIntensity(int8_t intensity) { Intensity_a(intensity); }

    void SetScale(uint8_t scale) { VIA_t1_cnt_lo = scale; }

    void Draw(const int8_t* const& vectorList, int8_t relx, int8_t rely, uint8_t scale) {
        SetScale(scale);
        Moveto_d(relx, rely);
        Draw_VLc(vectorList);
    }
} // namespace bios
