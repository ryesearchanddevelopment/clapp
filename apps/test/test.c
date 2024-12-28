#include "test.h"
#include "../../clay/clay.h"

void appTest(bool fullScreen, int widgetIndex)
{
    if (fullScreen)
    {
        CLAY(
            CLAY_ID("OuterContainer"),
            CLAY_RECTANGLE({.color = {43, 41, 51, 255}}),
            CLAY_LAYOUT({.layoutDirection = CLAY_TOP_TO_BOTTOM,
                         .sizing = {
                             .width = CLAY_SIZING_GROW(),
                             .height = CLAY_SIZING_GROW()},
                         .padding = {16, 16},
                         .childGap = 16}))
        {
            CLAY(
                CLAY_ID("HeaderBar"),
                CLAY_RECTANGLE({.color = {90, 90, 90, 255},
                                .cornerRadius = 8}),
                CLAY_LAYOUT({.sizing = {
                                 .height = CLAY_SIZING_FIXED(60),
                                 .width = CLAY_SIZING_GROW()},
                             .padding = {16},
                             .childGap = 16,
                             .childAlignment = {.y = CLAY_ALIGN_Y_CENTER}}))
            {
                CLAY_TEXT(CLAY_STRING("Header Item 1"), CLAY_TEXT_CONFIG({.fontSize = 24, .textColor = {255, 255, 255, 255}}));
                CLAY_TEXT(CLAY_STRING("Header Item 2"), CLAY_TEXT_CONFIG({.fontSize = 24, .textColor = {255, 255, 255, 255}}));
                CLAY_TEXT(CLAY_STRING("Header Item 3"), CLAY_TEXT_CONFIG({.fontSize = 24, .textColor = {255, 255, 255, 255}}));
            }
        }
    }
    else
    {
        CLAY(
            CLAY_IDI("WidgetContainer", widgetIndex),
            CLAY_RECTANGLE({}),
            CLAY_LAYOUT({.layoutDirection = CLAY_TOP_TO_BOTTOM,
                         .sizing = {
                             .width = CLAY_SIZING_GROW(),
                             .height = CLAY_SIZING_GROW()},
                         .padding = {16, 16},
                         .childGap = 16}))
        {
            CLAY_TEXT(CLAY_STRING("Hello, World!"), CLAY_TEXT_CONFIG({.fontSize = 24, .textColor = {255, 255, 255, 255}}));
        }
    }
}