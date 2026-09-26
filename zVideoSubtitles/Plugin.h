
// This file added in headers queue
// File: "Headers.h"

#define VIEW_VXMAX 8192
#define VIEW_VYMAX 8192

namespace GOTHIC_ENGINE {

    zCSubtitlePlayer    subtitlePlayer;
    oCBinkPlayer*       videoPlayer     = nullptr;
    bool                isDx11          = false;

    struct BINK
    {
        uint32 Width;
        uint32 Height;
        uint32 Frames;
        uint32 FrameNum;
        uint32 LastFrameNum;

        uint32 FrameRate;
        uint32 FrameRateDiv;

        // ...
        // Bink struct is actually much larger but this is all we need
    };

    HOOK Hook_oCBinkPlayer_OpenVideo PATCH(&oCBinkPlayer::OpenVideo, &oCBinkPlayer::OpenVideo_Hook);
    int oCBinkPlayer::OpenVideo_Hook(zSTRING filename) {

        videoPlayer = this;

        // Extract subtitles filename from video filename
        zSTRING subFilename = filename;
        subFilename.Upper();
        uint32 pos = subFilename.SearchReverse(".");
        if (pos)
        {
            subFilename.Cut(pos, 256);
        }
        subFilename += ".SRT";

        // Parse
        subtitlePlayer.ParseSubtitles(subFilename);

        // Call original method
        return THISCALL(Hook_oCBinkPlayer_OpenVideo)(filename);
    }

    HOOK Hook_zCRnd_D3D_Vid_Blit PATCH(&zCRnd_D3D::Vid_Blit, &zCRnd_D3D::Vid_Blit_Hook);
    void zCRnd_D3D::Vid_Blit_Hook(int forceFlip, struct tagRECT* sourceRect, struct tagRECT* destRect)
    {
        // Render subtitles
        if (videoPlayer && videoPlayer->IsPlaying())
        {
            BINK* bink = (BINK*)videoPlayer->mVideoHandle;

            if (isDx11)
            {
                // Workaround for GD3D11 because everything ever
                // made for Gothic fucking sucks!
                bink = *(BINK**)videoPlayer->mVideoHandle;
            }

            unsigned long currentFrame = (bink->FrameNum > 0) ? (bink->FrameNum - 1) : 0;
            unsigned long timeInMS = (currentFrame * bink->FrameRateDiv * 1000) / bink->FrameRate;

            subtitlePlayer.RenderSubtitles(timeInMS);
        }

        // Call original method
        THISCALL(Hook_zCRnd_D3D_Vid_Blit)(forceFlip, sourceRect, destRect);
    }

}