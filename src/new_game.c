#include "global.h"
#include "new_game.h"
#include "random.h"
#include "pokemon.h"
#include "roamer.h"
#include "pokemon_size_record.h"
#include "script.h"
#include "lottery_corner.h"
#include "play_time.h"
#include "mauville_old_man.h"
#include "match_call.h"
#include "lilycove_lady.h"
#include "load_save.h"
#include "pokeblock.h"
#include "dewford_trend.h"
#include "berry.h"
#include "rtc.h"
#include "easy_chat.h"
#include "event_data.h"
#include "money.h"
#include "trainer_hill.h"
#include "tv.h"
#include "coins.h"
#include "text.h"
#include "overworld.h"
#include "mail.h"
#include "battle_records.h"
#include "item.h"
#include "pokedex.h"
#include "apprentice.h"
#include "frontier_util.h"
#include "pokedex.h"
#include "save.h"
#include "link_rfu.h"
#include "main.h"
#include "contest.h"
#include "item_menu.h"
#include "pokemon_storage_system.h"
#include "string_util.h"
#include "pokemon_jump.h"
#include "decoration_inventory.h"
#include "secret_base.h"
#include "player_pc.h"
#include "field_specials.h"
#include "berry_powder.h"
#include "mystery_gift.h"
#include "union_room_chat.h"
#include "constants/map_groups.h"
#include "constants/items.h"
#include "constants/abilities.h"
#include "constants/moves.h"
#include "constants/pokemon.h"
#include "constants/species.h"
#include "tx_registered_items_menu.h"
#include "randomizer.h"

extern const u8 EventScript_ResetAllMapFlags[];

struct TestBoxMon;

static void ClearFrontierRecord(void);
static void WarpToTruck(void);
static void ResetMiniGamesRecords(void);
static void CreateTestBoxes(void);
static void CreateTestBoxMon(struct BoxPokemon *boxMon, const struct TestBoxMon *testMon);

struct TestBoxMon
{
    u16 species;
    u16 item;
    u16 moves[MAX_MON_MOVES];
    u8 abilityNum;
    u8 nature;
};

static const u8 sText_TestName[] = _("Test");
static const u8 sText_TestNameUpper[] = _("TEST");
static const u8 sText_TestBox2[] = _("TEST 2");
static const u8 sText_TestBox3[] = _("TEST 3");
static const u8 sText_DefaultBox3[] = _("Box3");

static const struct TestBoxMon sTestBoxMons[] =
{
    {SPECIES_NINETALES,          ITEM_HEAVY_DUTY_BOOTS, {MOVE_FLAMETHROWER, MOVE_SOLAR_BEAM, MOVE_SCORCHING_SANDS, MOVE_NASTY_PLOT}, 2, NATURE_TIMID},
    {SPECIES_NINETALES_ALOLA,    ITEM_LIGHT_CLAY,       {MOVE_BLIZZARD, MOVE_FREEZE_DRY, MOVE_MOONBLAST, MOVE_AURORA_VEIL}, 2, NATURE_TIMID},
    {SPECIES_ELECTIVIRE,         ITEM_LIFE_ORB,         {MOVE_DOUBLE_EDGE, MOVE_CLOSE_COMBAT, MOVE_ICE_PUNCH, MOVE_EARTHQUAKE}, 1, NATURE_JOLLY},
    {SPECIES_MAGNETON,           ITEM_EVIOLITE,         {MOVE_THUNDERBOLT, MOVE_FLASH_CANNON, MOVE_VOLT_SWITCH, MOVE_BODY_PRESS}, 2, NATURE_MODEST},
    {SPECIES_JOLTEON,            ITEM_LIFE_ORB,         {MOVE_THUNDERBOLT, MOVE_VOLT_SWITCH, MOVE_GRASS_KNOT, MOVE_EVOLUTION_BOOST}, 0, NATURE_TIMID},
    {SPECIES_UMBREON,            ITEM_LEFTOVERS,        {MOVE_BODY_PRESS, MOVE_FOUL_PLAY, MOVE_WISH, MOVE_EVOLUTION_BOOST}, 0, NATURE_IMPISH},
    {SPECIES_GLACEON,            ITEM_HEAVY_DUTY_BOOTS, {MOVE_ICE_BEAM, MOVE_FREEZE_DRY, MOVE_SURF, MOVE_EVOLUTION_BOOST}, 2, NATURE_TIMID},
    {SPECIES_LEAFEON,            ITEM_LIFE_ORB,         {MOVE_LEAF_BLADE, MOVE_HIGH_HORSEPOWER, MOVE_KNOCK_OFF, MOVE_EVOLUTION_BOOST}, 0, NATURE_JOLLY},
    {SPECIES_ESPEON,             ITEM_LIFE_ORB,         {MOVE_PSYCHIC, MOVE_DAZZLING_GLEAM, MOVE_SHADOW_BALL, MOVE_EVOLUTION_BOOST}, 1, NATURE_TIMID},
    {SPECIES_EEVEE,              ITEM_EVIOLITE,         {MOVE_EVOLUTION_BOOST, MOVE_STORED_POWER, MOVE_HYPER_VOICE, MOVE_BATON_PASS}, 2, NATURE_TIMID},
    {SPECIES_AERODACTYL,         ITEM_FOCUS_SASH,       {MOVE_HEAD_SMASH, MOVE_BRAVE_BIRD, MOVE_EARTHQUAKE, MOVE_DRAGON_DANCE}, 0, NATURE_JOLLY},
    {SPECIES_STEELIX,            ITEM_LIFE_ORB,         {MOVE_ROCK_SLIDE, MOVE_ICE_FANG, MOVE_CRUNCH, MOVE_EARTHQUAKE}, 2, NATURE_ADAMANT},
    {SPECIES_TYPHLOSION,         ITEM_CHOICE_SPECS,     {MOVE_ERUPTION, MOVE_FLAMETHROWER, MOVE_FOCUS_BLAST, MOVE_SCORCHING_SANDS}, 1, NATURE_TIMID},
    {SPECIES_TYPHLOSION_HISUI,   ITEM_CHOICE_SPECS,     {MOVE_INFERNAL_PARADE, MOVE_FLAMETHROWER, MOVE_FOCUS_BLAST, MOVE_SHADOW_BALL}, 2, NATURE_TIMID},
    {SPECIES_SWAMPERT,           ITEM_LEFTOVERS,        {MOVE_EARTHQUAKE, MOVE_LIQUIDATION, MOVE_ICE_PUNCH, MOVE_STEALTH_ROCK}, 2, NATURE_ADAMANT},
    {SPECIES_ROSERADE,           ITEM_LIFE_ORB,         {MOVE_VACUUM_WAVE, MOVE_SHOCK_WAVE, MOVE_MAGICAL_LEAF, MOVE_SLUDGE_BOMB}, 2, NATURE_MODEST},
    {SPECIES_MASQUERAIN,         ITEM_HEAVY_DUTY_BOOTS, {MOVE_QUIVER_DANCE, MOVE_HURRICANE, MOVE_BUG_BUZZ, MOVE_HYDRO_PUMP}, 0, NATURE_TIMID},
    {SPECIES_FROSMOTH,           ITEM_HEAVY_DUTY_BOOTS, {MOVE_QUIVER_DANCE, MOVE_ICE_BEAM, MOVE_BUG_BUZZ, MOVE_GIGA_DRAIN}, 2, NATURE_TIMID},
    {SPECIES_GLALIE,             ITEM_GLALITITE,        {MOVE_BOOMBURST, MOVE_HYPER_VOICE, MOVE_EARTH_POWER, MOVE_FREEZE_DRY}, 0, NATURE_TIMID},
    {SPECIES_TORTERRA,           ITEM_LEFTOVERS,        {MOVE_SHELL_SMASH, MOVE_EARTHQUAKE, MOVE_WOOD_HAMMER, MOVE_STONE_EDGE}, 2, NATURE_ADAMANT},
    {SPECIES_SERPERIOR,          ITEM_LEFTOVERS,        {MOVE_LEAF_STORM, MOVE_SLUDGE_BOMB, MOVE_SCORCHING_SANDS, MOVE_GLARE}, 2, NATURE_TIMID},
    {SPECIES_ZOROARK,            ITEM_FOCUS_SASH,       {MOVE_KNOCK_OFF, MOVE_SUCKER_PUNCH, MOVE_LOW_KICK, MOVE_SWORDS_DANCE}, 0, NATURE_JOLLY},
    {SPECIES_BRAVIARY_HISUI,     ITEM_LIFE_ORB,         {MOVE_ESPER_WING, MOVE_HURRICANE, MOVE_HEAT_WAVE, MOVE_CALM_MIND}, 2, NATURE_TIMID},
    {SPECIES_VIKAVOLT,           ITEM_LIFE_ORB,         {MOVE_BUG_BUZZ, MOVE_THUNDERBOLT, MOVE_ENERGY_BALL, MOVE_AGILITY}, 2, NATURE_MODEST},
    {SPECIES_MAWILE,             ITEM_LIFE_ORB,         {MOVE_PLAY_ROUGH, MOVE_IRON_HEAD, MOVE_SUCKER_PUNCH, MOVE_SWORDS_DANCE}, 0, NATURE_ADAMANT},
    {SPECIES_ALTARIA,            ITEM_ALTARIANITE,     {MOVE_BOOMBURST, MOVE_DRACO_METEOR, MOVE_FIRE_BLAST, MOVE_ROOST}, 0, NATURE_MODEST},
    {SPECIES_WHISCASH,           ITEM_LIFE_ORB,         {MOVE_LIQUIDATION, MOVE_EARTHQUAKE, MOVE_STONE_EDGE, MOVE_DRAGON_DANCE}, 2, NATURE_ADAMANT},
    {SPECIES_CLAYDOL,            ITEM_LEFTOVERS,        {MOVE_EARTH_POWER, MOVE_PSYCHIC, MOVE_RECOVER, MOVE_RAPID_SPIN}, 2, NATURE_BOLD},
    {SPECIES_DUSKNOIR,           ITEM_ASSAULT_VEST,     {MOVE_SHADOW_PUNCH, MOVE_DRAIN_PUNCH, MOVE_ICE_PUNCH, MOVE_THUNDER_PUNCH}, 1, NATURE_ADAMANT},
    {SPECIES_SPIRITOMB,          ITEM_LEFTOVERS,        {MOVE_WILL_O_WISP, MOVE_RECOVER, MOVE_FOUL_PLAY, MOVE_TAUNT}, 1, NATURE_BOLD},
    {SPECIES_HATTERENE,          ITEM_LEFTOVERS,        {MOVE_PSYCHIC, MOVE_DAZZLING_GLEAM, MOVE_MYSTICAL_FIRE, MOVE_CALM_MIND}, 1, NATURE_QUIET},
    {SPECIES_TOXTRICITY,         ITEM_THROAT_SPRAY,     {MOVE_BOOMBURST, MOVE_OVERDRIVE, MOVE_SLUDGE_WAVE, MOVE_VOLT_SWITCH}, 1, NATURE_MODEST},
    {SPECIES_FLYGON,             ITEM_LIFE_ORB,         {MOVE_EARTHQUAKE, MOVE_DRAGON_CLAW, MOVE_U_TURN, MOVE_DRAGON_DANCE}, 0, NATURE_ADAMANT},
    {SPECIES_ARBOK,              ITEM_LIFE_ORB,         {MOVE_DRAGON_DANCE, MOVE_CRUNCH, MOVE_POISON_FANG, MOVE_EARTHQUAKE}, 2, NATURE_JOLLY},
    {SPECIES_DREDNAW,            ITEM_LIFE_ORB,         {MOVE_DRAGON_DANCE, MOVE_JAW_LOCK, MOVE_LIQUIDATION, MOVE_STONE_IMPACT}, 2, NATURE_JOLLY},
    {SPECIES_SEVIPER,            ITEM_LIFE_ORB,         {MOVE_POISON_FANG, MOVE_CRUNCH, MOVE_EARTHQUAKE, MOVE_GUNK_SHOT}, 2, NATURE_ADAMANT},
    {SPECIES_SEISMITOAD,         ITEM_THROAT_SPRAY,     {MOVE_RESONANT_STRIKE, MOVE_BOOMBURST, MOVE_EARTHQUAKE, MOVE_SLUDGE_WAVE}, 2, NATURE_ADAMANT},
    {SPECIES_LURANTIS,           ITEM_LIFE_ORB,         {MOVE_FLORAL_SLASH, MOVE_SUPERPOWER, MOVE_KNOCK_OFF, MOVE_SYNTHESIS}, 2, NATURE_ADAMANT},
    {SPECIES_FEAROW,             ITEM_LIFE_ORB,         {MOVE_DRILL_PECK, MOVE_DRILL_RUN, MOVE_U_TURN, MOVE_ROOST}, 0, NATURE_JOLLY},
    {SPECIES_BEEDRILL,           ITEM_LIFE_ORB,         {MOVE_POISON_JAB, MOVE_MEGAHORN, MOVE_DRILL_RUN, MOVE_SWORDS_DANCE}, 1, NATURE_JOLLY},
    {SPECIES_ESCAVALIER,         ITEM_ASSAULT_VEST,     {MOVE_MEGAHORN, MOVE_SMART_STRIKE, MOVE_KNOCK_OFF, MOVE_DRILL_RUN}, 1, NATURE_ADAMANT},
    {SPECIES_MAGCARGO,           ITEM_LEFTOVERS,        {MOVE_FLAMETHROWER, MOVE_EARTH_POWER, MOVE_RECOVER, MOVE_WILL_O_WISP}, 2, NATURE_BOLD},
    {SPECIES_ARCANINE_HISUI,     ITEM_LIFE_ORB,         {MOVE_FLARE_BLITZ, MOVE_HEAD_SMASH, MOVE_EXTREME_SPEED, MOVE_WILD_CHARGE}, 2, NATURE_JOLLY},
    {SPECIES_WEAVILE,            ITEM_LIFE_ORB,         {MOVE_FAKE_OUT, MOVE_KNOCK_OFF, MOVE_ICE_SHARD, MOVE_TRIPLE_AXEL}, 2, NATURE_JOLLY},
    {SPECIES_PRIMARINA,          ITEM_THROAT_SPRAY,     {MOVE_HYPER_VOICE, MOVE_MOONBLAST, MOVE_PSYCHIC, MOVE_ICE_BEAM}, 2, NATURE_MODEST},
    {SPECIES_SLAKING,            ITEM_LIFE_ORB,         {MOVE_DOUBLE_EDGE, MOVE_EARTHQUAKE, MOVE_KNOCK_OFF, MOVE_SLACK_OFF}, 0, NATURE_JOLLY},
    {SPECIES_NOIVERN,            ITEM_CHOICE_SPECS,     {MOVE_BOOMBURST, MOVE_DRACO_METEOR, MOVE_FLAMETHROWER, MOVE_U_TURN}, 2, NATURE_TIMID},
    {SPECIES_SWELLOW,            ITEM_FLAME_ORB,        {MOVE_BOOMBURST, MOVE_HURRICANE, MOVE_FOCUS_BLAST, MOVE_NASTY_PLOT}, 1, NATURE_TIMID},
    {SPECIES_FLAREON,            ITEM_LIFE_ORB,         {MOVE_DOUBLE_EDGE, MOVE_FLARE_BLITZ, MOVE_PLAY_ROUGH, MOVE_QUICK_ATTACK}, 1, NATURE_ADAMANT},
    {SPECIES_ROTOM_HEAT,         ITEM_HEAVY_DUTY_BOOTS, {MOVE_OVERHEAT, MOVE_INCINERATE, MOVE_VOLT_SWITCH, MOVE_WILL_O_WISP}, 0, NATURE_TIMID},
    {SPECIES_ROTOM_WASH,         ITEM_LEFTOVERS,        {MOVE_HYDRO_PUMP, MOVE_WATER_PULSE, MOVE_VOLT_SWITCH, MOVE_PAIN_SPLIT}, 0, NATURE_BOLD},
    {SPECIES_ROTOM_FROST,        ITEM_HEAVY_DUTY_BOOTS, {MOVE_BLIZZARD, MOVE_FREEZE_DRY, MOVE_VOLT_SWITCH, MOVE_NASTY_PLOT}, 0, NATURE_TIMID},
    {SPECIES_ROTOM_MOW,          ITEM_CHOICE_SPECS,     {MOVE_LEAF_STORM, MOVE_GIGA_DRAIN, MOVE_VOLT_SWITCH, MOVE_THUNDERBOLT}, 0, NATURE_TIMID},
    {SPECIES_ROTOM_FAN,          ITEM_HEAVY_DUTY_BOOTS, {MOVE_HURRICANE, MOVE_AIR_SLASH, MOVE_VOLT_SWITCH, MOVE_THUNDER_WAVE}, 0, NATURE_TIMID},
    {SPECIES_DRAPION,            ITEM_LIFE_ORB,         {MOVE_POISON_JAB, MOVE_KNOCK_OFF, MOVE_EARTHQUAKE, MOVE_SWORDS_DANCE}, 1, NATURE_JOLLY},
    {SPECIES_LIEPARD,            ITEM_FOCUS_SASH,       {MOVE_KNOCK_OFF, MOVE_SUCKER_PUNCH, MOVE_TAUNT, MOVE_THUNDER_WAVE}, 2, NATURE_JOLLY},
    {SPECIES_DRUDDIGON,          ITEM_LIFE_ORB,         {MOVE_DRAGON_RUSH, MOVE_IRON_HEAD, MOVE_FIRE_PUNCH, MOVE_ROCK_SLIDE}, 1, NATURE_BRAVE},
    {SPECIES_SANDSLASH,          ITEM_LIFE_ORB,         {MOVE_EARTHQUAKE, MOVE_STONE_IMPACT, MOVE_SWORDS_DANCE, MOVE_RAPID_SPIN}, 2, NATURE_JOLLY},
    {SPECIES_SANDSLASH_ALOLA,    ITEM_LIFE_ORB,         {MOVE_EARTHQUAKE, MOVE_ICICLE_CRASH, MOVE_IRON_HEAD, MOVE_SWORDS_DANCE}, 0, NATURE_JOLLY},
    {SPECIES_RAPIDASH,           ITEM_LIFE_ORB,         {MOVE_FLARE_BLITZ, MOVE_WILD_CHARGE, MOVE_DOUBLE_EDGE, MOVE_MORNING_SUN}, 0, NATURE_JOLLY},
    {SPECIES_RAPIDASH_GALAR,     ITEM_LIFE_ORB,         {MOVE_PLAY_ROUGH, MOVE_MEGAHORN, MOVE_PSYCHO_CUT, MOVE_SWORDS_DANCE}, 0, NATURE_JOLLY},
    {SPECIES_ELECTRODE,          ITEM_THROAT_SPRAY,     {MOVE_HYPER_VOICE, MOVE_THUNDERBOLT, MOVE_VOLT_SWITCH, MOVE_LIGHT_SCREEN}, 2, NATURE_TIMID},
    {SPECIES_ELECTRODE_HISUI,    ITEM_THROAT_SPRAY,     {MOVE_HYPER_VOICE, MOVE_THUNDERBOLT, MOVE_VOLT_SWITCH, MOVE_GIGA_DRAIN}, 2, NATURE_TIMID},
    {SPECIES_EXPLOUD,            ITEM_THROAT_SPRAY,     {MOVE_BOOMBURST, MOVE_HYPER_VOICE, MOVE_FLAMETHROWER, MOVE_SURF}, 2, NATURE_MODEST},
    {SPECIES_AGGRON,             ITEM_LIFE_ORB,         {MOVE_STONE_IMPACT, MOVE_IRON_HEAD, MOVE_FIRE_PUNCH, MOVE_DRAGON_DANCE}, 2, NATURE_ADAMANT},
    {SPECIES_MANECTRIC,          ITEM_LIFE_ORB,         {MOVE_THUNDERBOLT, MOVE_GRASS_KNOT, MOVE_VOLT_SWITCH, MOVE_FLAMETHROWER}, 0, NATURE_TIMID},
    {SPECIES_KROOKODILE,         ITEM_LIFE_ORB,         {MOVE_EARTHQUAKE, MOVE_KNOCK_OFF, MOVE_STONE_IMPACT, MOVE_DRAGON_DANCE}, 0, NATURE_JOLLY},
    {SPECIES_ARCHEOPS,           ITEM_FOCUS_SASH,       {MOVE_ACROBATICS, MOVE_STONE_EDGE, MOVE_EARTHQUAKE, MOVE_U_TURN}, 0, NATURE_JOLLY},
    {SPECIES_TALONFLAME,         ITEM_HEAVY_DUTY_BOOTS, {MOVE_BRAVE_BIRD, MOVE_FLARE_BLITZ, MOVE_ROOST, MOVE_U_TURN}, 2, NATURE_JOLLY},
    {SPECIES_AVALUGG,            ITEM_LEFTOVERS,        {MOVE_BODY_PRESS, MOVE_HEAVY_SLAM, MOVE_AVALANCHE, MOVE_RECOVER}, 2, NATURE_IMPISH},
    {SPECIES_DECIDUEYE,          ITEM_LIFE_ORB,         {MOVE_SPIRIT_SHACKLE, MOVE_SEED_BOMB, MOVE_BULLET_SEED, MOVE_SHADOW_BALL}, 2, NATURE_ADAMANT},
};

EWRAM_DATA bool8 gDifferentSaveFile = FALSE;
EWRAM_DATA bool8 gEnableContestDebugging = FALSE;

static const struct ContestWinner sContestWinnerPicDummy =
{
    .monName = _(""),
    .trainerName = _("")
};

void SetTrainerId(u32 trainerId, u8 *dst)
{
    dst[0] = trainerId;
    dst[1] = trainerId >> 8;
    dst[2] = trainerId >> 16;
    dst[3] = trainerId >> 24;
}

u32 GetTrainerId(u8 *trainerId)
{
    return (trainerId[3] << 24) | (trainerId[2] << 16) | (trainerId[1] << 8) | (trainerId[0]);
}

void CopyTrainerId(u8 *dst, u8 *src)
{
    s32 i;
    for (i = 0; i < TRAINER_ID_LENGTH; i++)
        dst[i] = src[i];
}

static void InitPlayerTrainerId(void)
{
    u32 trainerId = (Random() << 16) | GetGeneratedTrainerIdLower();
    SetTrainerId(trainerId, gSaveBlock2Ptr->playerTrainerId);
}

// L=A isnt set here for some reason.
static void SetDefaultOptions(void)
{
    gSaveBlock2Ptr->optionsTextSpeed = OPTIONS_TEXT_SPEED_FAST;
    gSaveBlock2Ptr->optionsWindowFrameType = 0;
    gSaveBlock2Ptr->optionsSound = OPTIONS_SOUND_STEREO;
    gSaveBlock2Ptr->optionsBattleStyle = OPTIONS_BATTLE_STYLE_SET;
    gSaveBlock2Ptr->optionsBattleSceneOff = FALSE;
    gSaveBlock2Ptr->regionMapZoom = FALSE;
}

static void ClearPokedexFlags(void)
{
    gUnusedPokedexU8 = 0;
    memset(&gSaveBlock1Ptr->dexCaught, 0, sizeof(gSaveBlock1Ptr->dexCaught));
    memset(&gSaveBlock1Ptr->dexSeen, 0, sizeof(gSaveBlock1Ptr->dexSeen));
}

void ClearAllContestWinnerPics(void)
{
    s32 i;

    ClearContestWinnerPicsInContestHall();

    // Clear Museum paintings
    for (i = MUSEUM_CONTEST_WINNERS_START; i < NUM_CONTEST_WINNERS; i++)
        gSaveBlock1Ptr->contestWinners[i] = sContestWinnerPicDummy;
}

static void ClearFrontierRecord(void)
{
    CpuFill32(0, &gSaveBlock2Ptr->frontier, sizeof(gSaveBlock2Ptr->frontier));

    gSaveBlock2Ptr->frontier.opponentNames[0][0] = EOS;
    gSaveBlock2Ptr->frontier.opponentNames[1][0] = EOS;
}

static void WarpToTruck(void)
{
    SetWarpDestination(MAP_GROUP(INSIDE_OF_TRUCK), MAP_NUM(INSIDE_OF_TRUCK), WARP_ID_NONE, -1, -1);
    WarpIntoMap();
}

void Sav2_ClearSetDefault(void)
{
    ClearSav2();
    SetDefaultOptions();
}

void ResetMenuAndMonGlobals(void)
{
    gDifferentSaveFile = FALSE;
    ResetPokedexScrollPositions();
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ResetBagScrollPositions();
    ResetPokeblockScrollPositions();
}

void NewGameInitData(void)
{
    if (gSaveFileStatus == SAVE_STATUS_EMPTY || gSaveFileStatus == SAVE_STATUS_CORRUPT)
        RtcReset();

    gDifferentSaveFile = TRUE;
    gSaveBlock2Ptr->encryptionKey = 0;
    ZeroPlayerPartyMons();
    ZeroEnemyPartyMons();
    ResetPokedex();
    ClearFrontierRecord();
    ClearSav1();
    ClearSav3();
    ClearAllMail();
    gSaveBlock2Ptr->specialSaveWarpFlags = 0;
    gSaveBlock2Ptr->gcnLinkFlags = 0;
    InitPlayerTrainerId();
    PlayTimeCounter_Reset();
    ClearPokedexFlags();
    InitEventData();
    ClearTVShowData();
    ResetGabbyAndTy();
    ClearSecretBases();
    ClearBerryTrees();
    SetMoney(&gSaveBlock1Ptr->money, 3000);
    SetCoins(0);
    ResetLinkContestBoolean();
    ResetGameStats();
    ClearAllContestWinnerPics();
    ClearPlayerLinkBattleRecords();
    InitSeedotSizeRecord();
    InitLotadSizeRecord();
    gPlayerPartyCount = 0;
    ZeroPlayerPartyMons();
    ResetPokemonStorageSystem();
    CreateTestBoxes();
    if (StringCompare(gSaveBlock2Ptr->playerName, sText_TestName) == 0
     || StringCompare(gSaveBlock2Ptr->playerName, sText_TestNameUpper) == 0)
        VarSet(VAR_TEST_BOX_VERSION, 1);
    DeactivateAllRoamers();
    ClearBag();
    if (StringCompare(gSaveBlock2Ptr->playerName, sText_TestName) == 0
     || StringCompare(gSaveBlock2Ptr->playerName, sText_TestNameUpper) == 0)
        AddBagItem(ITEM_MEGA_RING, 1);
    NewGameInitPCItems();
    TxRegItemsMenu_RegisteredItemsMenuNewGame();
    ClearPokeblocks();
    ClearDecorationInventories();
    InitEasyChatPhrases();
    SetMauvilleOldMan();
    InitDewfordTrend();
    ResetFanClub();
    ResetLotteryCorner();
    WarpToTruck();
    RunScriptImmediately(EventScript_ResetAllMapFlags);
    ResetMiniGamesRecords();
    InitUnionRoomChatRegisteredTexts();
    InitLilycoveLady();
    ResetAllApprenticeData();
    ClearRankingHallRecords();
    InitMatchCallCounters();
    ClearMysteryGift();
    WipeTrainerNameRecords();
    ResetTrainerHillResults();
    ResetContestLinkResults();
    #if (RANDOMIZER_AVAILABLE == TRUE) && (RANDOMIZER_DYNAMIC_SPECIES == TRUE)
        PreloadRandomizationTables();
    #endif
    memset(gSaveBlock1Ptr->dexNavSearchLevels, 0, sizeof(gSaveBlock1Ptr->dexNavSearchLevels));
    gSaveBlock1Ptr->dexNavChain = 0;
    gSaveBlock2Ptr->autoRun = FALSE;
    gSaveBlock1Ptr->whiteOuts = 0;
}

static void CreateTestBoxes(void)
{
    u32 i;

    if (StringCompare(gSaveBlock2Ptr->playerName, sText_TestName) != 0
     && StringCompare(gSaveBlock2Ptr->playerName, sText_TestNameUpper) != 0)
        return;

    StringCopy(gPokemonStoragePtr->boxNames[0], sText_TestNameUpper);
    StringCopy(gPokemonStoragePtr->boxNames[1], sText_TestBox2);
    StringCopy(gPokemonStoragePtr->boxNames[2], sText_TestBox3);

    for (i = 0; i < ARRAY_COUNT(sTestBoxMons); i++)
        CreateTestBoxMon(&gPokemonStoragePtr->boxes[i / IN_BOX_COUNT][i % IN_BOX_COUNT], &sTestBoxMons[i]);
}

static void CreateTestBoxMon(struct BoxPokemon *boxMon, const struct TestBoxMon *testMon)
{
    u32 value;
    u32 j;

    CreateBoxMon(boxMon, testMon->species, 50, MAX_PER_STAT_IVS, TRUE, testMon->nature, OT_ID_PLAYER_ID, 0);
    value = testMon->item;
    SetBoxMonData(boxMon, MON_DATA_HELD_ITEM, &value);
    value = testMon->abilityNum;
    SetBoxMonData(boxMon, MON_DATA_ABILITY_NUM, &value);

    for (j = 0; j < MAX_MON_MOVES; j++)
    {
        value = testMon->moves[j];
        SetBoxMonData(boxMon, MON_DATA_MOVE1 + j, &value);
        value = gMovesInfo[testMon->moves[j]].pp;
        SetBoxMonData(boxMon, MON_DATA_PP1 + j, &value);
    }
}

static bool32 IsNewTestBoxSpecies(u32 species)
{
    switch (species)
    {
    case SPECIES_ARBOK:
    case SPECIES_DREDNAW:
    case SPECIES_SEVIPER:
    case SPECIES_SEISMITOAD:
    case SPECIES_LURANTIS:
    case SPECIES_FEAROW:
    case SPECIES_BEEDRILL:
    case SPECIES_ESCAVALIER:
    case SPECIES_MAGCARGO:
    case SPECIES_ARCANINE_HISUI:
    case SPECIES_PRIMARINA:
    case SPECIES_SLAKING:
    case SPECIES_DRAPION:
    case SPECIES_LIEPARD:
    case SPECIES_DRUDDIGON:
    case SPECIES_SANDSLASH:
    case SPECIES_SANDSLASH_ALOLA:
    case SPECIES_RAPIDASH:
    case SPECIES_RAPIDASH_GALAR:
    case SPECIES_ELECTRODE:
    case SPECIES_ELECTRODE_HISUI:
    case SPECIES_EXPLOUD:
    case SPECIES_AGGRON:
    case SPECIES_MANECTRIC:
    case SPECIES_KROOKODILE:
    case SPECIES_ARCHEOPS:
    case SPECIES_TALONFLAME:
    case SPECIES_AVALUGG:
    case SPECIES_DECIDUEYE:
        return TRUE;
    default:
        return FALSE;
    }
}

// Only fill vacant slots in the original TEST boxes. Keep all other stored
// Pokemon, including their custom moves and held items, untouched.
void UpdateTestBoxesForExistingSave(void)
{
    u32 box, slot, i;
    bool32 complete = TRUE;
    bool32 canUseThirdBox = FALSE;

    if ((StringCompare(gSaveBlock2Ptr->playerName, sText_TestName) != 0
      && StringCompare(gSaveBlock2Ptr->playerName, sText_TestNameUpper) != 0)
     || VarGet(VAR_TEST_BOX_VERSION) >= 1
     || StringCompare(gPokemonStoragePtr->boxNames[0], sText_TestNameUpper) != 0
     || StringCompare(gPokemonStoragePtr->boxNames[1], sText_TestBox2) != 0)
        return;

    if (StringCompare(gPokemonStoragePtr->boxNames[2], sText_TestBox3) == 0)
        canUseThirdBox = TRUE;
    else if (StringCompare(gPokemonStoragePtr->boxNames[2], sText_DefaultBox3) == 0)
    {
        canUseThirdBox = TRUE;
        for (slot = 0; slot < IN_BOX_COUNT; slot++)
            if (GetBoxMonData(&gPokemonStoragePtr->boxes[2][slot], MON_DATA_SANITY_HAS_SPECIES))
                canUseThirdBox = FALSE;
        if (canUseThirdBox)
            StringCopy(gPokemonStoragePtr->boxNames[2], sText_TestBox3);
    }

    for (box = 0; box < 2; box++)
    {
        for (slot = 0; slot < IN_BOX_COUNT; slot++)
        {
            struct BoxPokemon *mon = &gPokemonStoragePtr->boxes[box][slot];
            u32 species = GetBoxMonData(mon, MON_DATA_SPECIES);
            u32 value;

            if (species == SPECIES_ALTARIA && GetBoxMonData(mon, MON_DATA_HELD_ITEM) == ITEM_THROAT_SPRAY)
            {
                value = ITEM_ALTARIANITE;
                SetBoxMonData(mon, MON_DATA_HELD_ITEM, &value);
            }
            else if (species == SPECIES_FLYGON
                  && GetBoxMonData(mon, MON_DATA_HELD_ITEM) == ITEM_LIFE_ORB
                  && GetBoxMonData(mon, MON_DATA_ABILITY_NUM) == 1)
            {
                value = 0;
                SetBoxMonData(mon, MON_DATA_ABILITY_NUM, &value);
            }
            else if (species == SPECIES_WEAVILE && GetBoxMonData(mon, MON_DATA_MOVE4) == MOVE_LOW_KICK)
            {
                value = MOVE_FAKE_OUT;
                SetBoxMonData(mon, MON_DATA_MOVE4, &value);
                value = gMovesInfo[MOVE_FAKE_OUT].pp;
                SetBoxMonData(mon, MON_DATA_PP4, &value);
            }
            else if (species == SPECIES_ARCANINE_HISUI
                  && GetBoxMonData(mon, MON_DATA_HELD_ITEM) == ITEM_LIFE_ORB
                  && GetBoxMonData(mon, MON_DATA_ABILITY_NUM) == 1)
            {
                value = 2;
                SetBoxMonData(mon, MON_DATA_ABILITY_NUM, &value);
            }
        }
    }

    for (i = 0; i < ARRAY_COUNT(sTestBoxMons); i++)
    {
        bool32 found = FALSE;
        if (!IsNewTestBoxSpecies(sTestBoxMons[i].species))
            continue;

        for (box = 0; box < (canUseThirdBox ? 3 : 2) && !found; box++)
            for (slot = 0; slot < IN_BOX_COUNT; slot++)
                if (GetBoxMonData(&gPokemonStoragePtr->boxes[box][slot], MON_DATA_SPECIES) == sTestBoxMons[i].species)
                {
                    found = TRUE;
                    break;
                }

        if (!found)
        {
            for (box = 0; box < (canUseThirdBox ? 3 : 2) && !found; box++)
                for (slot = 0; slot < IN_BOX_COUNT; slot++)
                    if (!GetBoxMonData(&gPokemonStoragePtr->boxes[box][slot], MON_DATA_SANITY_HAS_SPECIES))
                    {
                        CreateTestBoxMon(&gPokemonStoragePtr->boxes[box][slot], &sTestBoxMons[i]);
                        found = TRUE;
                        break;
                    }
            if (!found)
                complete = FALSE;
        }
    }

    if (complete)
        VarSet(VAR_TEST_BOX_VERSION, 1);
}

static void ResetMiniGamesRecords(void)
{
    CpuFill16(0, &gSaveBlock2Ptr->berryCrush, sizeof(struct BerryCrush));
    SetBerryPowder(&gSaveBlock2Ptr->berryCrush.berryPowderAmount, 0);
    ResetPokemonJumpRecords();
    CpuFill16(0, &gSaveBlock2Ptr->berryPick, sizeof(struct BerryPickingResults));
}
