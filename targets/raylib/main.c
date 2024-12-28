#define CLAY_IMPLEMENTATION
#include "../../clay/clay.h"
#include "../../clay/renderers/raylib/clay_renderer_raylib.c"
#include "apps.h"

#define RAYLIB_VECTOR2_TO_CLAY_VECTOR2(vector) \
    (Clay_Vector2) { .x = vector.x, .y = vector.y }

typedef struct
{
    Clay_Vector2 clickOrigin;
    Clay_Vector2 positionOrigin;
    bool mouseDown;
} ScrollbarData;

ScrollbarData scrollbarData = (ScrollbarData){};

bool debugEnabled = false;

void UpdateDrawFrame()
{
    Vector2 mouseWheelDelta = GetMouseWheelMoveV();
    float mouseWheelX = mouseWheelDelta.x;
    float mouseWheelY = mouseWheelDelta.y;

    if (IsKeyPressed(KEY_D))
    {
        debugEnabled = !debugEnabled;
        Clay_SetDebugModeEnabled(debugEnabled);
    }
    //----------------------------------------------------------------------------------
    // Handle scroll containers
    Clay_Vector2 mousePosition = RAYLIB_VECTOR2_TO_CLAY_VECTOR2(GetMousePosition());
    Clay_SetPointerState(mousePosition, IsMouseButtonDown(0) && !scrollbarData.mouseDown);
    Clay_SetLayoutDimensions((Clay_Dimensions){(float)GetScreenWidth(), (float)GetScreenHeight()});
    if (!IsMouseButtonDown(0))
    {
        scrollbarData.mouseDown = false;
    }

    if (IsMouseButtonDown(0) && !scrollbarData.mouseDown && Clay_PointerOver(Clay__HashString(CLAY_STRING("ScrollBar"), 0, 0)))
    {
        Clay_ScrollContainerData scrollContainerData = Clay_GetScrollContainerData(Clay__HashString(CLAY_STRING("MainContent"), 0, 0));
        scrollbarData.clickOrigin = mousePosition;
        scrollbarData.positionOrigin = *scrollContainerData.scrollPosition;
        scrollbarData.mouseDown = true;
    }
    else if (scrollbarData.mouseDown)
    {
        Clay_ScrollContainerData scrollContainerData = Clay_GetScrollContainerData(Clay__HashString(CLAY_STRING("MainContent"), 0, 0));
        if (scrollContainerData.contentDimensions.height > 0)
        {
            Clay_Vector2 ratio = (Clay_Vector2){
                scrollContainerData.contentDimensions.width / scrollContainerData.scrollContainerDimensions.width,
                scrollContainerData.contentDimensions.height / scrollContainerData.scrollContainerDimensions.height,
            };
            if (scrollContainerData.config.vertical)
            {
                scrollContainerData.scrollPosition->y = scrollbarData.positionOrigin.y + (scrollbarData.clickOrigin.y - mousePosition.y) * ratio.y;
            }
            if (scrollContainerData.config.horizontal)
            {
                scrollContainerData.scrollPosition->x = scrollbarData.positionOrigin.x + (scrollbarData.clickOrigin.x - mousePosition.x) * ratio.x;
            }
        }
    }

    Clay_UpdateScrollContainers(true, (Clay_Vector2){mouseWheelX, mouseWheelY}, GetFrameTime());
    // Generate the auto layout for rendering
    double currentTime = GetTime();
    Clay_BeginLayout();
    renderApps(IsMouseButtonPressed(MOUSE_LEFT_BUTTON));
    Clay_RenderCommandArray renderCommands = Clay_EndLayout();
    printf("layout time: %f microseconds\n", (GetTime() - currentTime) * 1000 * 1000);
    // RENDERING ---------------------------------
    //    currentTime = GetTime();
    BeginDrawing();
    ClearBackground(BLACK);
    Clay_Raylib_Render(renderCommands);
    EndDrawing();
    //    printf("render time: %f ms\n", (GetTime() - currentTime) * 1000);

    //----------------------------------------------------------------------------------
}

bool reinitializeClay = false;

void HandleClayErrors(Clay_ErrorData errorData)
{
    printf("%s", errorData.errorText.chars);
    if (errorData.errorType == CLAY_ERROR_TYPE_ELEMENTS_CAPACITY_EXCEEDED)
    {
        reinitializeClay = true;
        Clay_SetMaxElementCount(Clay__maxElementCount * 2);
    }
    else if (errorData.errorType == CLAY_ERROR_TYPE_TEXT_MEASUREMENT_CAPACITY_EXCEEDED)
    {
        reinitializeClay = true;
        Clay_SetMaxMeasureTextCacheWordCount(Clay__maxMeasureTextCacheWordCount * 2);
    }
}


int main(void)
{
    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
    Clay_SetMeasureTextFunction(Raylib_MeasureText);
    Clay_Initialize(clayMemory, (Clay_Dimensions){(float)GetScreenWidth(), (float)GetScreenHeight()}, (Clay_ErrorHandler){HandleClayErrors});
    Clay_Raylib_Initialize(1024, 768, "Clay - Raylib Renderer Example", FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);
    Raylib_fonts[0] = (Raylib_Font){
        .font = LoadFontEx("targets/raylib/resources/Roboto-Regular.ttf", 48, 0, 400),
        .fontId = 0,
    };
    SetTextureFilter(Raylib_fonts[0].font.texture, TEXTURE_FILTER_BILINEAR);

    Raylib_fonts[0] = (Raylib_Font){
        .font = LoadFontEx("targets/raylib/resources/Roboto-Regular.ttf", 32, 0, 400),
        .fontId = 0,
    };
    SetTextureFilter(Raylib_fonts[0].font.texture, TEXTURE_FILTER_BILINEAR);

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        if (reinitializeClay)
        {
            Clay_SetMaxElementCount(8192);
            totalMemorySize = Clay_MinMemorySize();
            clayMemory = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
            Clay_Initialize(clayMemory, (Clay_Dimensions){(float)GetScreenWidth(), (float)GetScreenHeight()}, (Clay_ErrorHandler){HandleClayErrors});
            reinitializeClay = false;
        }
        UpdateDrawFrame();
    }
}
