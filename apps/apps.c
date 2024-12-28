#include "apps.h"
#include "../clay/clay.h"
#include "test.h"

typedef void (*app) (bool, int);

app apps[] = {
    appTest,
    appTest,
    appTest,    appTest,
    appTest,
    appTest,
    appTest,
    appTest,
    appTest,
    appTest,
};

int appCount = sizeof(apps) / sizeof(app);
int currentApp = -1;

void renderApps(bool pressed) {
    if(currentApp >= appCount) {
        currentApp = 0;
    }

    if (currentApp < 0)
    {
        // Display the launcer
        CLAY(
            CLAY_ID("Launcher"),
            CLAY_RECTANGLE({ .color = { 43, 41, 51, 255 } }),
            CLAY_LAYOUT({
                .layoutDirection = CLAY_LEFT_TO_RIGHT,
                .sizing = {
                    .width = CLAY_SIZING_GROW(),
                    .height = CLAY_SIZING_GROW()
                },
                .padding = { 16, 16 },
                .childGap = 16
            })
        ) {
            for (int i = 0; i < appCount; i++)
            {
                CLAY(
                    CLAY_IDI("AppButton", i),
                    CLAY_RECTANGLE({
                        .color = { 90, 90, 90, 255 },
                        .cornerRadius = 8
                    }),
                    CLAY_LAYOUT({
                        .sizing = {
                            .height = CLAY_SIZING_FIXED(120),
                            .width = CLAY_SIZING_FIXED(120)
                        },
                        .padding = { 16 },
                        .childGap = 16,
                        .childAlignment = {
                            .y = CLAY_ALIGN_Y_CENTER
                        }
                    })
                ) {
                    if(Clay_PointerOver(Clay__HashString(CLAY_STRING("AppButton"), i, 0))) {
                        if(pressed) {
                            currentApp = i;
                        }
                    }
                    apps[i](false, i);
                }
            }
        }
    }
    else
    {
        // Render the current app
        apps[currentApp](true, currentApp);
    }
}