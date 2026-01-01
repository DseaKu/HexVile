#include "ui_handler.h"
#include "GFX_manager.h"
#include "defines.h"
#include "enums.h"
#include "font_handler.h"
#include "frame_context.h"
#include "hex_tile_grid.h"
#include "item_handler.h"
#include "raylib.h"
#include "raymath.h"
#include "resource.h"
#include "texture.h"
#include "ui_layout.h"
#include <string>

// --- Constructors ---
UI_Handler::UI_Handler() {
  // Initialize Layout Configuration
  toolBarLayout.maxSlots = conf::TOOLBAR_N_ITEM_SLOTS;
  toolBarLayout.padding = conf::TOOLBAR_PADDING;

  // Dynamic calculation based on texture options
  float renderedSlotSize = tex::opts::ITEM_SLOT.scale * tex::size::TILE;

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

  ToolBarInventorySpace = conf::TOOLBAR_INVENTORY_SPACE;
}

// --- Core Lifecycle ---
void UI_Handler::Update() {
  if (frameContext->inputs.commands.toggleInventory) {
    ToggleInventory();
  }

  // Update item bar properties
  this->slotSize = tex::opts::ITEM_SLOT.scale * tex::size::TILE;
  this->itemBarWidth =
      (conf::TOOLBAR_N_ITEM_SLOTS * this->slotSize) +
      ((conf::TOOLBAR_N_ITEM_SLOTS - 1) * ui_layout::ITEM_SLOT_SPACING);
  this->xStartPos = frameContext->screen.center.x -
                    (this->itemBarWidth / 2.0f) + (this->slotSize / 2.0f);
}

void UI_Handler::UpdateScreenSize(int width, int height) {
  // Update Center X
  toolBarLayout.posX = (float)width / 2.0f;

  // Calculate Rect Position
  float rectX = toolBarLayout.posX - (toolBarLayout.width / 2.0f);
  float rectY = height - toolBarLayout.rect.height - toolBarLayout.bottomMargin;

  // Update Y Center for slots
  toolBarLayout.posY = rectY + (toolBarLayout.rect.height / 2.0f);

  toolBarLayout.rect = {rectX, rectY, toolBarLayout.width,
                        toolBarLayout.rect.height};
}

mouseMask::id UI_Handler::UpdateMouseMask() {
  if (isToolBarActive && CheckCollisionPointRec(frameContext->screen.mousePos,
                                                toolBarLayout.rect)) {
    return mouseMask::TOOL_BAR;
  } else {
    return mouseMask::GROUND;
  }
}

void UI_Handler::ToggleInventory() {
  if (this->isInventoryOpen) {
    isInventoryOpen = false;
  } else {
    isInventoryOpen = true;
  }
}

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

// --- Getters & Queries ---
bool UI_Handler::GetToolBarAvailability() { return isToolBarActive; }
Rectangle UI_Handler::GetToolBarRect() { return toolBarLayout.rect; }

int UI_Handler::GetToolBarSelection() {
  int curSelection = frameContext->selToolBarSlot;
  if (curSelection < 0 || curSelection >= conf::TOOLBAR_N_ITEM_SLOTS) {
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
  if (curSelection >= conf::TOOLBAR_N_ITEM_SLOTS) {
    curSelection = toolBarSlotBuffer;
  }

  // Check if player clicked Tool Bar
  if (frameContext->inputs.mouseClick.left &&
      frameContext->mouseMask == mouseMask::TOOL_BAR) {

    // Local coordinate in the toolbar
    // toolBarLayout.rect.x is the left edge of the toolbar
    float localX = frameContext->screen.mousePos.x - toolBarLayout.rect.x;

    if (localX < 0)
      return 0;

    int slotIndex = (int)(localX / toolBarLayout.slotSize);

    if (slotIndex >= 0 && slotIndex < toolBarLayout.maxSlots) {
      curSelection = slotIndex;
    }
  }
  return curSelection;
}

// --- Graphics / Backbuffer ---
void UI_Handler::LoadBackBuffer() {
  LoadHighlightGFX();

  if (isToolBarActive) {
    float yStart = frameContext->screen.bot - tex::size::TILE -
                   ui_layout::TOOL_BAR_BOT_MARGIN;

    LoadItemGridGFX(1, yStart);
  }

  if (isInventoryOpen) {
    // LoadInventoryBackgroundGFX();
    // LoadInventoryItemsGFX();
  }
}

// --- Private Methods ---
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

void UI_Handler::LoadHighlightTileGFX() {
  if (!hexGrid)
    return;
  HexCoord coord = hexGrid->PointToHexCoord(frameContext->world.mousePos);
  hexGrid->DrawTile(coord, tex::atlas::TILE_HIGHLIGHTED, drawMask::GROUND1);
}

void UI_Handler::LoadHighlightResourceGFX(rsrc::ID id) {
  if (!frameContext || !frameContext->world.hoveredTile)
    return;

  // Can be changed to indicate 'not in range'
  Color col = WHITE;

  // If player is in interact range change color indicator to yellow
  Vector2 curMousePos = frameContext->world.mousePos;
  if (Vector2Distance(curMousePos, frameContext->world.playerPos) <
      conf::INTERACT_DISTANCE_PLAYER) {
    col = YELLOW;
  }

  const rsrc::Object &rsrc = frameContext->world.hoveredTile->rsrc;
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
  // Calculate inventory grid height to find center Y
  int cols = conf::INVENTORY_CELL_COLS;
  int rows = conf::INVENTORY_SLOTS / cols;

  tex::Opts opts = tex::opts::ITEM_SLOT;
  float slotSize = opts.scale * tex::size::TILE;
  float spacing = 10.0f * conf::UI_SCALE;
  float gridHeight = rows * slotSize + (rows - 1) * spacing;

  // Center X is screen center
  float centerX = conf::SCREEN_CENTER.x;

  // Center Y is middle of the grid area above toolbar
  float centerY = toolBarLayout.rect.y - conf::TOOLBAR_INVENTORY_SPACE -
                  (gridHeight / 2.0f);

  graphicsManager->LoadTextureToBackbuffer(
      drawMask::UI_0, tex::atlas::INVENTORY, {centerX, centerY},
      tex::opts::IVENTORY);
}

void UI_Handler::LoadInventoryItemsGFX() {
  // int cols = conf::INVENTORY_CELL_COLS;
  // int rows = conf::INVENTORY_SLOTS / cols;
  //
  // // Use same scale as toolbar for consistency
  // tex::Opts opts = tex::opts::ITEM_SLOT_BACKGROUND;
  // float slotSize = opts.scale * tex::size::TILE;
  // float spacing = 10.0f * conf::UI_SCALE;
  //
  // float gridWidth = cols * slotSize + (cols - 1) * spacing;
  // float gridHeight = rows * slotSize + (rows - 1) * spacing;
  //
  // // Center X
  // float startX = conf::SCREEN_CENTER.x - (gridWidth / 2.0f);
  //
  // // Y Position: Above Toolbar
  // // toolbarRect.y is the top edge of the toolbar
  // float startY =
  //     toolBarLayout.rect.y - gridHeight - conf::TOOLBAR_INVENTORY_SPACE;
  //
  // // Shift to center of the first slot (since startX/Y is top-left edge)
  // float firstSlotCenterX = startX + (slotSize / 2.0f);
  // float firstSlotCenterY = startY + (slotSize / 2.0f);
  //
  // for (int i = 0; i < conf::INVENTORY_SLOTS; ++i) {
  //   int col = i % cols;
  //   int row = i / cols;
  //
  //   float centerX = firstSlotCenterX + col * (slotSize + spacing);
  //   float centerY = firstSlotCenterY + row * (slotSize + spacing);
  //
  //   Vector2 pos = {centerX, centerY};
  //
  //   const ItemStack *item = itemHandler->GetInventoryItemPointer(i);
  //
  //   LoadItemSlotGFX(item, pos, false);
  // }
}

void UI_Handler::LoadItemGridGFX(int row, float yPos) {

  for (int i = 0; i < conf::ITEM_GRID_COLS; i++) {

    // Get item
    ItemStack *item = itemHandler->GetToolBarItemPointer(i);

    float xOffset = i * (this->slotSize + ui_layout::ITEM_SLOT_SPACING);
    Vector2 dst = {this->xStartPos + xOffset, yPos};

    // Load  item slot background
    this->LoadItemSlotBackgroundGFX(dst);

    if (i == frameContext->selToolBarSlot) {
      this->LoadItemSlotHighlightGFX(dst);
    }

    if (item->itemID == item::NULL_ID) {
      continue;
    }
    // Load item icon
    this->LoadItemIconGFX(tex::lut::ITEM_TEXTURE_COORDS.at(item->itemID), dst);

    // Load item num
    this->LoadItemCountGFX(item, dst);
  }
}

void UI_Handler::LoadItemSlotHighlightGFX(Vector2 dst) {

  graphicsManager->LoadTextureToBackbuffer(drawMask::UI_1,
                                           tex::atlas::ITEM_SLOT_HIGHLIGHTED,
                                           dst, tex::opts::ITEM_SLOT);
}

void UI_Handler::LoadItemSlotBackgroundGFX(Vector2 dst) {

  graphicsManager->LoadTextureToBackbuffer(drawMask::UI_1,
                                           tex::atlas::ITEM_SLOT_BACKGROUND,
                                           dst, tex::opts::ITEM_SLOT);
}

void UI_Handler::LoadItemIconGFX(tex::atlas::Coords taCoords, Vector2 dst) {
  graphicsManager->LoadTextureToBackbuffer(drawMask::UI_2, taCoords, dst,
                                           tex::opts::ITEM_ICON);
}

void UI_Handler::LoadItemCountGFX(const ItemStack *item, Vector2 dst) {

  dst = {dst.x + ui_layout::ITEM_NUM_X_OFFSET,
         dst.y + ui_layout::ITEM_NUM_Y_OFFSET};

  // Draw Numbers (Right-aligned to bottom-right corner)
  std::string num_str = std::to_string(item->count);
  int digitIndex = 0;

  // Iterate backwards to draw from right to left
  for (auto it = num_str.rbegin(); it != num_str.rend(); ++it) {

    // Convert char -> int
    int digit = *it - '0';

    // Move left for each digit
    dst.x -= digitIndex * tex::opts::NUMBERS.scale * tex::size::TILE;

    // Shift to correct number
    tex::atlas::Coords num = tex::atlas::NUMBER;
    num.x += digit;

    graphicsManager->LoadTextureToBackbuffer(drawMask::UI_2, num, dst,
                                             tex::opts::NUMBERS);
    digitIndex++;
  }
}
