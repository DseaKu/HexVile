#include "ui_handler.h"
#include "GFX_manager.h"
#include "defines.h"
#include "enums.h"
#include "font_handler.h"
#include "frame_context.h"
#include "hex_tile_grid.h"
#include "raylib.h"
#include "raymath.h"
#include "resource.h"
#include "texture.h"
#include <string>

// --- Initialization ---
UI_Handler::UI_Handler() {
  // Initialize Layout Configuration
  toolBarLayout.maxSlots = conf::TOOLBAR_SLOTS;
  toolBarLayout.padding = conf::TOOLBAR_PADDING;

  // Dynamic calculation based on texture options
  float renderedSlotSize =
      tex::opts::ITEM_SLOT_BACKGROUND.scale * tex::size::TILE;
  
  // Calculate slot size including spacing
  // spacing is arbitrary, using padding as gap between slots
  float spacing = 10.0f * conf::UI_SCALE; 
  
  toolBarLayout.contentSize = (int)renderedSlotSize;
  toolBarLayout.slotSize = (int)(renderedSlotSize + spacing);

  toolBarLayout.itemScale = conf::TOOLBAR_ITEM_ICON_SCALE;
  toolBarLayout.scale = conf::UI_SCALE;
  toolBarLayout.bottomMargin = conf::TOOLBAR_BOTTOM_MARGIN;

  // Calculate Toolbar Width (total width of all slots + spacing)
  toolBarLayout.width = (toolBarLayout.maxSlots * toolBarLayout.slotSize); 

  // Initial Position (Center of the toolbar block)
  // X: Screen Center
  // Y: Bottom of screen - margin - half height (since origin is center)
  toolBarLayout.posX = conf::SCREEN_CENTER.x;
  
  // For the Rect, we still need top-left coordinates for collision detection
  float rectWidth = toolBarLayout.width;
  float rectHeight = renderedSlotSize; // Height is just one slot
  float rectX = toolBarLayout.posX - (rectWidth / 2.0f);
  float rectY = conf::SCREEN_HEIGHT - rectHeight - toolBarLayout.bottomMargin;

  // Store the Y center for rendering slots
  toolBarLayout.posY = rectY + (rectHeight / 2.0f);

  toolBarLayout.rect = {rectX, rectY, rectWidth, rectHeight};

  // State
  isToolBarActive = false;
  isInventoryOpen = false;

  // Dependencies
  graphicsManager = nullptr;
  itemHandler = nullptr;
  fontHandler = nullptr;
  hexGrid = nullptr;
}

// --- Core ---
void UI_Handler::Update() {
  if (frameContext->inputs.commands.toggleInventory) {
    ToggleInventory();
  }
}

void UI_Handler::LoadBackBuffer() {
  LoadHighlightGFX();
  if (isToolBarActive) {
    LoadToolBarGFX();
  }
  if (isInventoryOpen) {
    LoadInventoryBackgroundGFX();
    LoadInventoryItemsGFX();
  }
}

void UI_Handler::ToggleInventory() {
  if (this->isInventoryOpen) {
    isInventoryOpen = false;
  } else {
    isInventoryOpen = true;
  }
}

void UI_Handler::LoadHighlightGFX() {
  // Get to hightlithning object
  item::id itemID =
      itemHandler->GetToolBarItemType(frameContext->selToolBarSlot);
  if (itemID == item::AXE) {
    // LoadHighlightResourceGFX(rsrc::ID_TREE);
  }

  if (itemID == item::SET_GRASS) {
    LoadHighlightTileGFX();
  }
  if (itemID == item::SET_WATER) {
    LoadHighlightTileGFX();
  }
  if (itemID == item::SET_DIRT) {
    LoadHighlightTileGFX();
  }
}

void UI_Handler::UpdateScreenSize(int width, int height) {
  // Update Center X
  toolBarLayout.posX = (float)width / 2.0f;
  
  // Calculate Rect Position
  float rectX = toolBarLayout.posX - (toolBarLayout.width / 2.0f);
  float rectY = height - toolBarLayout.rect.height - toolBarLayout.bottomMargin;

  // Update Y Center for slots
  toolBarLayout.posY = rectY + (toolBarLayout.rect.height / 2.0f);

  toolBarLayout.rect = {rectX, rectY,
                        toolBarLayout.width, toolBarLayout.rect.height};
}

mouseMask::id UI_Handler::UpdateMouseMask() {
  if (isToolBarActive && CheckCollisionPointRec(frameContext->screenPos.mouse,
                                                toolBarLayout.rect)) {
    return mouseMask::TOOL_BAR;
  } else {
    return mouseMask::GROUND;
  }
}
// --- Input & Queries ---
bool UI_Handler::GetToolBarAvailability() { return isToolBarActive; }
Rectangle UI_Handler::GetToolBarRect() { return toolBarLayout.rect; }

// --- Setters ---
void UI_Handler::SetGFX_Manager(GFX_Manager *p) { graphicsManager = p; }
void UI_Handler::SetItemHandler(ItemHandler *p) { itemHandler = p; }
void UI_Handler::SetFontHandler(FontHandler *p) { fontHandler = p; }
void UI_Handler::SetHexGrid(HexGrid *p) { hexGrid = p; }
void UI_Handler::SetToolBarActive(bool is_active) {
  isToolBarActive = is_active;
}

void UI_Handler::SetFrameContext(const frame::Context *frameContext) {
  this->frameContext = frameContext;
}

// --- Load GFX Data ---
void UI_Handler::LoadHighlightTileGFX() {
  if (!hexGrid)
    return;
  HexCoord coord = hexGrid->PointToHexCoord(frameContext->worldPos.mouse);
  hexGrid->DrawTile(coord, tex::atlas::TILE_HIGHLIGHTED, drawMask::GROUND1);
}

void UI_Handler::LoadHighlightResourceGFX(rsrc::ID id) {
  if (!frameContext || !frameContext->worldPos.hoveredTile)
    return;

  // Can be changed to indicate 'not in range'
  Color col = WHITE;

  // If player is in interact range change color indicator to yellow
  Vector2 curMousePos = frameContext->worldPos.mouse;
  if (Vector2Distance(curMousePos, frameContext->worldPos.player) <
      conf::INTERACT_DISTANCE_PLAYER) {
    col = YELLOW;
  }

  const rsrc::Object &rsrc = frameContext->worldPos.hoveredTile->rsrc;
  if (rsrc.id == id) {
    // Get position of resource
    Vector2 rsrcPos = rsrc.worldPos;

    if (Vector2Distance(rsrcPos, curMousePos) < conf::INTERACT_DISTANCE_MOUSE) {
      graphicsManager->LoadTextureToBackbuffer(
          drawMask::ON_GROUND, tex::atlas::TREE, rsrcPos,
          {.color = Fade(col, conf::HIGHLIGHT_ALPHA),
           .srcHeight = tex::size::DOUBLE_TILE});
    }
  }
}

void UI_Handler::LoadInventoryBackgroundGFX() {

  graphicsManager->LoadTextureToBackbuffer(
      drawMask::UI_0, tex::atlas::INVENTORY, frameContext->screenPos.center,
      tex::opts::IVENTORY);
}

void UI_Handler::LoadInventoryItemsGFX() {
  int cols = conf::INVENTORY_CELL_COLS;
  int rows = conf::INVENTORY_SLOTS / cols;
  float cellSize = conf::INVENTORY_CELL_SIZE * conf::UI_SCALE;

  float totalWidth = cols * cellSize;
  float totalHeight = rows * cellSize;

  float startX = conf::SCREEN_CENTER.x - (totalWidth / 2.0f);
  float startY = conf::SCREEN_CENTER.y - (totalHeight / 2.0f);

  for (int y = 0; y < rows; ++y) {
    for (int x = 0; x < cols; ++x) {
      Rectangle dstRect = {startX + (x * cellSize), startY + (y * cellSize),
                           cellSize, cellSize};
    }
  }
  // Draw item slots
}
void UI_Handler::LoadToolBarGFX() {

  for (int i = 0; i < toolBarLayout.maxSlots; i++) {
    LoadItemSlotGFX(i);
  }
}

void UI_Handler::LoadItemSlotGFX(int slotIndex) {
  // toolBarLayout.rect.x is the left edge of the toolbar
  float startX = toolBarLayout.rect.x;
  
  // Center of this specific slot
  float centerX = startX + (slotIndex * toolBarLayout.slotSize) + (toolBarLayout.slotSize / 2.0f);
  float centerY = toolBarLayout.posY; 

  Vector2 dst = {centerX, centerY};
  tex::Opts opts = tex::opts::ITEM_SLOT_BACKGROUND;
  float renderedSize = opts.scale * tex::size::TILE;

  // Load Background
  graphicsManager->LoadTextureToBackbuffer(
      drawMask::UI_0, tex::atlas::ITEM_SLOT_BACKGROUND, dst, opts);

  if (slotIndex == frameContext->selToolBarSlot) {
    graphicsManager->LoadTextureToBackbuffer(
        drawMask::UI_0, tex::atlas::ITEM_SLOT_BACKGROUND_HIGHLIGHTED, dst,
        opts);
  }

  //  Load Content
  ItemStack *itemStack = itemHandler->GetToolBarItemPointer(slotIndex);
  if (itemStack && itemStack->itemID != item::NULL_ID) {
    // We still define slotRect as the bounding box for the content 
    // centered at (centerX, centerY) with size (renderedSize, renderedSize)
    Rectangle slotRect = {centerX - (renderedSize / 2.0f), 
                          centerY - (renderedSize / 2.0f), 
                          renderedSize, renderedSize};
    LoadItemIconGFX(slotIndex, slotRect);
    LoadItemCountGFX(slotIndex, slotRect);
  }
}

void UI_Handler::LoadItemIconGFX(int slotIndex, Rectangle slotRect) {
  ItemStack *itemStack = itemHandler->GetToolBarItemPointer(slotIndex);

  item::id itemID = itemStack->itemID;
  tex::atlas::Coords taCoords = tex::atlas::ITEM_TEXTURE_COORDS.at(itemID);

  // Calculate shrunk size for icon
  float iconSize = slotRect.width * toolBarLayout.itemScale;
  
  // Center of the slot
  Vector2 dst = {slotRect.x + slotRect.width / 2.0f, 
                 slotRect.y + slotRect.height / 2.0f};
                 
  tex::Opts opts = tex::opts::ITEM_ICON;
  float iconScale = (iconSize / tex::size::TILE) * opts.scale;
  
  opts.scale = iconScale;

  graphicsManager->LoadTextureToBackbuffer(drawMask::UI_0, taCoords, dst, opts);
}

void UI_Handler::LoadItemCountGFX(int slotIndex, Rectangle slotRect) {
  ItemStack *item = itemHandler->GetToolBarItemPointer(slotIndex);

  // Re-calculate the icon rect to position numbers correctly relative to it?
  // Original code used the icon rect for numbers too.

  float newWidth = slotRect.width * toolBarLayout.itemScale;
  float newHeight = slotRect.height * toolBarLayout.itemScale;
  float offsetX = (slotRect.width - newWidth) / 2.0f;
  float offsetY = (slotRect.height - newHeight) / 2.0f;

  Rectangle iconRect = {slotRect.x + offsetX, slotRect.y + offsetY, newWidth,
                        newHeight};

  // Draw Numbers (Right-aligned to bottom-right corner)
  std::string num_str = std::to_string(item->count);
  int digitIndex = 0;

  // Iterate backwards to draw from right to left
  for (auto it = num_str.rbegin(); it != num_str.rend(); ++it) {

    // Convert char -> int
    int digit = *it - '0';
    Vector2 dst = {iconRect.x + iconRect.width, iconRect.y + iconRect.height};

    // Move left for each digit
    dst.x -= digitIndex * tex::opts::NUMBERS.scale * tex::size::TILE;

    // Shift to correct number
    tex::atlas::Coords num = tex::atlas::NUMBER;
    num.x += digit;

    graphicsManager->LoadTextureToBackbuffer(drawMask::UI_1, num, dst,
                                             tex::opts::NUMBERS);
    digitIndex++;
  }
}

// --- Getter ---
int UI_Handler::GetToolBarSelection() {
  int curSelection = frameContext->selToolBarSlot;
  if (curSelection < 0 || curSelection >= conf::TOOLBAR_SLOTS) {
    curSelection = 0;
  }
  frame::InputCommands keyPress = frameContext->inputs.commands;

  int toolBarSlotBuffer = curSelection;
  if (keyPress.slot0)
    curSelection = 0;
  if (keyPress.slot1)
    curSelection = 1;
  if (keyPress.slot2)
    curSelection = 2;
  if (keyPress.slot3)
    curSelection = 3;
  if (keyPress.slot4)
    curSelection = 4;
  if (keyPress.slot5)
    curSelection = 5;
  if (keyPress.slot6)
    curSelection = 6;
  if (keyPress.slot7)
    curSelection = 7;
  if (keyPress.slot8)
    curSelection = 8;
  if (keyPress.slot9)
    curSelection = 9;

  // Revert selection if the toolbar slot is out of range
  if (curSelection >= conf::TOOLBAR_SLOTS) {
    curSelection = toolBarSlotBuffer;
  }

  // Check if player clicked Tool Bar
  if (frameContext->inputs.mouseClick.left &&
      frameContext->mouseMask == mouseMask::TOOL_BAR) {

    // Local coordinate in the toolbar
    // toolBarLayout.rect.x is the left edge of the toolbar
    float localX = frameContext->screenPos.mouse.x - toolBarLayout.rect.x;

    if (localX < 0)
      return 0;

    int slotIndex = (int)(localX / toolBarLayout.slotSize);

    if (slotIndex >= 0 && slotIndex < toolBarLayout.maxSlots) {
      curSelection = slotIndex;
    }
  }
  return curSelection;
}
