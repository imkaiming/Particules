#pragma once

#include <juce_graphics/juce_graphics.h>

using color = juce::Colour;

namespace particules
{
    namespace coloursv3
    {
        inline const color violetBleu{0xff6563ff};
        inline const color grisAnthracite{0xff2e2f33};
        inline const color perleBlanc{0xffefefef};
        inline const color grisMoyen{0xffa0a0a0};
    }

    namespace colours
    {
        namespace panel
        {
            inline const color outerPanel{0xff1d1d1c};
            inline const color audioFilePanel{0xff1e1e1e};
            inline const color controlPanel{0xff282929};
            inline const color contourPanel{0xff454444};

        }
        namespace accent
        {

            inline const color whiteAccent{0xfffafafa};
            inline const color lightgreyAccent{0xffd6d6d6};
            inline const color yellowAccent{0xffedc95};
            inline const color redAccent{0xffd64655};
            inline const color blueAccent{0xff51b3ff};
            inline const color purpleAccent{0xff9184ee};
        }

        //inline const color grey{0xff0a0a0a}; // https://www.color-hex.com/color/0a0a0a
        inline const color deepBlack{0xff1f1f1f}; // https://www.color-hex.com/color/1f1f1f

        // black tone
        //inline const color smokyBlack{0xff100c08};
        inline const color grey{0xffa0a0a0};
        inline const color midGrey{0xff666666};
        inline const color darkGrey{0xff353535};
        inline const color black{0xff141414};

        // accent colo
        inline const color green{0xff52b788};
        inline const color lavender{0xffccccff};
        inline const color blue{0xff6dd4ff};
        inline const color cream{0xfff6f9e4};
        inline const color red{0xffe5383b};
        inline const color pink{0xffff8fa3};
        inline const color jungleGreen{0xff2AAA8A};
        inline const color brightBlue{0xff0096FF};
    }
}