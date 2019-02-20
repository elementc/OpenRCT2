/*****************************************************************************
 * Copyright (c) 2014-2018 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "../common.h"
#include "../peep/Peep.h"
#include "../ride/Vehicle.h"
#include "SpriteBase.h"

#define SPRITE_INDEX_NULL 0xFFFF
#define MAX_SPRITES 10000
#define NUM_SPRITE_LISTS 6

enum SPRITE_IDENTIFIER
{
    SPRITE_IDENTIFIER_VEHICLE = 0,
    SPRITE_IDENTIFIER_PEEP = 1,
    SPRITE_IDENTIFIER_MISC = 2,
    SPRITE_IDENTIFIER_LITTER = 3,
    SPRITE_IDENTIFIER_NULL = 255
};

enum SPRITE_LIST
{
    SPRITE_LIST_NULL,
    SPRITE_LIST_TRAIN,
    SPRITE_LIST_PEEP,
    SPRITE_LIST_MISC,
    SPRITE_LIST_LITTER,
    SPRITE_LIST_UNKNOWN,
};

struct rct_sprite_generic : rct_sprite_common
{
    uint16_t frame;
};

struct rct_litter : rct_sprite_common
{
    uint32_t creationTick;
};

struct rct_balloon : rct_sprite_generic
{
    uint16_t popped;
    uint8_t time_to_move;
    uint8_t colour;

    void Update();
    void Pop();
    void Press();
};

struct rct_duck : rct_sprite_generic
{
    int16_t target_x;
    int16_t target_y;
    uint8_t state;

    void UpdateFlyToWater();
    void UpdateSwim();
    void UpdateDrink();
    void UpdateDoubleDrink();
    void UpdateFlyAway();
    uint32_t GetFrameImage(int32_t direction) const;
    void Invalidate();
    void Remove();
    void MoveTo(int16_t x, int16_t y, int16_t z);
};

struct rct_jumping_fountain : rct_sprite_generic
{
    uint8_t num_ticks_alive;
    uint8_t fountain_flags;
    int16_t target_x;
    int16_t target_y;
    uint16_t iteration;
};

struct rct_money_effect : rct_sprite_common
{
    uint16_t move_delay;
    uint8_t num_movements;
    uint8_t vertical;
    money32 value;
    int16_t offset_x;
    uint16_t wiggle;
};

struct rct_crashed_vehicle_particle : rct_sprite_generic
{
    uint16_t time_to_live;
    uint8_t colour[2];
    uint16_t crashed_sprite_base;
    int16_t velocity_x;
    int16_t velocity_y;
    int16_t velocity_z;
    int32_t acceleration_x;
    int32_t acceleration_y;
    int32_t acceleration_z;
};

struct rct_crash_splash : rct_sprite_generic
{
};

struct rct_steam_particle : rct_sprite_generic
{
    uint16_t time_to_move;
};

#pragma pack(push, 1)
/**
 * Sprite structure.
 * size: 0x0100
 */
union rct_sprite
{
    uint8_t pad_00[0x100];
    rct_sprite_generic generic;
    rct_peep peep;
    rct_litter litter;
    rct_vehicle vehicle;
    rct_balloon balloon;
    rct_duck duck;
    rct_jumping_fountain jumping_fountain;
    rct_money_effect money_effect;
    rct_crashed_vehicle_particle crashed_vehicle_particle;
    rct_crash_splash crash_splash;
    rct_steam_particle steam_particle;

    bool IsBalloon();
    bool IsDuck();
    bool IsPeep();
    rct_balloon* AsBalloon();
    rct_duck* AsDuck();
    rct_peep* AsPeep();
};
assert_struct_size(rct_sprite, 0x100);

struct rct_sprite_checksum
{
    std::array<uint8_t, 20> raw;

    std::string ToString() const;
};

#pragma pack(pop)

enum
{
    SPRITE_MISC_STEAM_PARTICLE,
    SPRITE_MISC_MONEY_EFFECT,
    SPRITE_MISC_CRASHED_VEHICLE_PARTICLE,
    SPRITE_MISC_EXPLOSION_CLOUD,
    SPRITE_MISC_CRASH_SPLASH,
    SPRITE_MISC_EXPLOSION_FLARE,
    SPRITE_MISC_JUMPING_FOUNTAIN_WATER,
    SPRITE_MISC_BALLOON,
    SPRITE_MISC_DUCK,
    SPRITE_MISC_JUMPING_FOUNTAIN_SNOW
};

enum
{
    SPRITE_FLAGS_IS_CRASHED_VEHICLE_SPRITE = 1 << 7,
    SPRITE_FLAGS_PEEP_VISIBLE = 1 << 8,  // Peep is eligible to show in summarized guest list window (is inside park?)
    SPRITE_FLAGS_PEEP_FLASHING = 1 << 9, // Deprecated: Use sprite_set_flashing/sprite_get_flashing instead.
};

enum
{
    LITTER_TYPE_SICK,
    LITTER_TYPE_SICK_ALT,
    LITTER_TYPE_EMPTY_CAN,
    LITTER_TYPE_RUBBISH,
    LITTER_TYPE_EMPTY_BURGER_BOX,
    LITTER_TYPE_EMPTY_CUP,
    LITTER_TYPE_EMPTY_BOX,
    LITTER_TYPE_EMPTY_BOTTLE,
    LITTER_TYPE_EMPTY_BOWL_RED,
    LITTER_TYPE_EMPTY_DRINK_CARTON,
    LITTER_TYPE_EMPTY_JUICE_CUP,
    LITTER_TYPE_EMPTY_BOWL_BLUE,
};

rct_sprite* try_get_sprite(size_t spriteIndex);
rct_sprite* get_sprite(size_t sprite_idx);

extern uint16_t gSpriteListHead[6];
extern uint16_t gSpriteListCount[6];
extern uint16_t gSpriteSpatialIndex[0x10001];

extern const rct_string_id litterNames[12];

rct_sprite* create_sprite(uint8_t bl);
void reset_sprite_list();
void reset_sprite_spatial_index();
void sprite_clear_all_unused();
void move_sprite_to_list(rct_sprite* sprite, uint8_t cl);
void sprite_misc_update_all();
void sprite_move(int16_t x, int16_t y, int16_t z, rct_sprite* sprite);
void sprite_set_coordinates(int16_t x, int16_t y, int16_t z, rct_sprite* sprite);
void invalidate_sprite_0(rct_sprite* sprite);
void invalidate_sprite_1(rct_sprite* sprite);
void invalidate_sprite_2(rct_sprite* sprite);
void sprite_remove(rct_sprite* sprite);
void litter_create(int32_t x, int32_t y, int32_t z, int32_t direction, int32_t type);
void litter_remove_at(int32_t x, int32_t y, int32_t z);
void sprite_misc_explosion_cloud_create(int32_t x, int32_t y, int32_t z);
void sprite_misc_explosion_flare_create(int32_t x, int32_t y, int32_t z);
uint16_t sprite_get_first_in_quadrant(int32_t x, int32_t y);
void sprite_position_tween_store_a();
void sprite_position_tween_store_b();
void sprite_position_tween_all(float nudge);
void sprite_position_tween_restore();
void sprite_position_tween_reset();

///////////////////////////////////////////////////////////////
// Balloon
///////////////////////////////////////////////////////////////
void create_balloon(int32_t x, int32_t y, int32_t z, int32_t colour, bool isPopped);
void balloon_update(rct_balloon* balloon);
void game_command_balloon_press(
    int32_t* eax, int32_t* ebx, int32_t* ecx, int32_t* edx, int32_t* esi, int32_t* edi, int32_t* ebp);

///////////////////////////////////////////////////////////////
// Duck
///////////////////////////////////////////////////////////////
void create_duck(int32_t targetX, int32_t targetY);
void duck_update(rct_duck* duck);
void duck_press(rct_duck* duck);
void duck_remove_all();
uint32_t duck_get_frame_image(const rct_duck* duck, int32_t direction);

///////////////////////////////////////////////////////////////
// Money effect
///////////////////////////////////////////////////////////////
void money_effect_create(money32 value);
void money_effect_create_at(money32 value, int32_t x, int32_t y, int32_t z, bool vertical);
void money_effect_update(rct_money_effect* moneyEffect);
rct_string_id money_effect_get_string_id(const rct_money_effect* sprite, money32* outValue);

///////////////////////////////////////////////////////////////
// Crash particles
///////////////////////////////////////////////////////////////
void crashed_vehicle_particle_create(rct_vehicle_colour colours, int32_t x, int32_t y, int32_t z);
void crashed_vehicle_particle_update(rct_crashed_vehicle_particle* particle);
void crash_splash_create(int32_t x, int32_t y, int32_t z);
void crash_splash_update(rct_crash_splash* splash);

rct_sprite_checksum sprite_checksum();

void sprite_set_flashing(rct_sprite* sprite, bool flashing);
bool sprite_get_flashing(rct_sprite* sprite);
int32_t check_for_sprite_list_cycles(bool fix);
int32_t check_for_spatial_index_cycles(bool fix);
int32_t fix_disjoint_sprites();

#endif
