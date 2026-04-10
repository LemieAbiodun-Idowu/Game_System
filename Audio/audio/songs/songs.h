#pragma once
#include <stdint.h>
#include "ace_attorney_objection_2001.h"
#include "apollo_justice_fate_smeared_by_tricks_and_gimmicks.h"
#include "apollo_justice_suspense.h"
#include "blue_badger_i_want_to_defend.h"
#include "crystal_eusine.h"
#include "encounter_champion_cynthia.h"
#include "gsc_blackthorn_city.h"
#include "gsc_credits.h"
#include "gsc_ecruteak_city.h"
#include "gsc_game_corner.h"
#include "gsc_ruins_of_alph.h"
#include "gsc_wild_battle.h"
#include "kay_faraday_great_truth_burglar.h"
#include "lynne_targeted_redhead.h"
#include "masters_ex_champion_mashup.h"
#include "mystery_dungeon_rescue_team_base.h"
#include "mystery_dungeon_rescue_team_boss.h"
#include "pallet_town.h"
#include "pearl_fey_with_pearly.h"
#include "pokemon_colosseum_miror_b_retro_groove.h"
#include "pokemon_xd_vs_miror_b.h"
#include "pressing_pursuit_cornered.h"
#include "pressing_pursuit_questioned.h"
#include "quartz_quadrant.h"
#include "rby_cerulean_city.h"
#include "rby_game_corner.h"
#include "rby_gym_leader.h"
#include "rby_pokemon_center.h"
#include "rby_surfing.h"
#include "ss_aqua_advance.h"
#include "tetris_theme.h"
#include "the_field_of_hopes_and_dreams.h"
#include "trials_and_tribulations_confess_the_truth_2004.h"
#include "trials_and_tribulations_objection_2004.h"
#include "trials_and_tribulations_the_bitter_taste_of_truth.h"
#include "trials_and_tribulations_the_fragrance_of_dark_coffee.h"
#include "turnabout_sisters.h"


struct Song{
    const char* name;
    const uint16_t (*ch1)[3];
    const uint16_t (*ch2)[3];
    const uint16_t (*ch3)[3];
    const uint16_t ch1_len;
    const uint16_t ch2_len;
    const uint16_t ch3_len;
};

const Song songList[] = {
{objection2001Name, objection2001_ch1, objection2001_ch2, objection2001_ch3, objection2001_ch1_len, objection2001_ch2_len, objection2001_ch3_len},
{aj_tricks_and_gimmicksName, aj_tricks_and_gimmicks_ch1, aj_tricks_and_gimmicks_ch2, 0, aj_tricks_and_gimmicks_ch1_len, aj_tricks_and_gimmicks_ch2_len, 0},
{aj_suspenseName, aj_suspense_ch1, aj_suspense_ch2, 0, aj_suspense_ch1_len, aj_suspense_ch2_len, 0},
{aa1_blue_badgerName, aa1_blue_badger_ch1, aa1_blue_badger_ch2, 0, aa1_blue_badger_ch1_len, aa1_blue_badger_ch2_len, 0},
{crystal_eusineName, crystal_eusine_ch1, crystal_eusine_ch2, 0, crystal_eusine_ch1_len, crystal_eusine_ch2_len, 0},
{encounter_champion_cynthiaName, encounter_champion_cynthia_ch1, encounter_champion_cynthia_ch2, 0, encounter_champion_cynthia_ch1_len, encounter_champion_cynthia_ch2_len, 0},
{gsc_blackthorn_cityName, gsc_blackthorn_city_ch1, gsc_blackthorn_city_ch2, 0, gsc_blackthorn_city_ch1_len, gsc_blackthorn_city_ch2_len, 0},
{gsc_creditsName, gsc_credits_ch1, gsc_credits_ch2, 0, gsc_credits_ch1_len, gsc_credits_ch2_len, 0},
{gsc_ecruteak_cityName, gsc_ecruteak_city_ch1, gsc_ecruteak_city_ch2, 0, gsc_ecruteak_city_ch1_len, gsc_ecruteak_city_ch2_len, 0},
{gsc_game_cornerName, gsc_game_corner_ch1, gsc_game_corner_ch2, 0, gsc_game_corner_ch1_len, gsc_game_corner_ch2_len, 0},
{gsc_ruins_of_alphName, gsc_ruins_of_alph_ch1, gsc_ruins_of_alph_ch2, 0, gsc_ruins_of_alph_ch1_len, gsc_ruins_of_alph_ch2_len, 0},
{gsc_wild_battleName, gsc_wild_battle_ch1, gsc_wild_battle_ch2, 0, gsc_wild_battle_ch1_len, gsc_wild_battle_ch2_len, 0},
{kay_faraday_great_truth_burglarName, kay_faraday_great_truth_burglar_ch1, kay_faraday_great_truth_burglar_ch2, 0, kay_faraday_great_truth_burglar_ch1_len, kay_faraday_great_truth_burglar_ch2_len, 0},
{lynne_a_targeted_redheadName, lynne_a_targeted_redhead_ch1, lynne_a_targeted_redhead_ch2, lynne_a_targeted_redhead_ch3, lynne_a_targeted_redhead_ch1_len, lynne_a_targeted_redhead_ch2_len, lynne_a_targeted_redhead_ch3_len},
{champions_mashupName, champions_mashup_ch1, champions_mashup_ch2, 0, champions_mashup_ch1_len, champions_mashup_ch2_len, 0},
{mystery_dungeon_rescue_team_baseName, mystery_dungeon_rescue_team_base_ch1, mystery_dungeon_rescue_team_base_ch2, 0, mystery_dungeon_rescue_team_base_ch1_len, mystery_dungeon_rescue_team_base_ch2_len, 0},
{mystery_dungeon_rescue_team_bossName, mystery_dungeon_rescue_team_boss_ch1, mystery_dungeon_rescue_team_boss_ch2, 0, mystery_dungeon_rescue_team_boss_ch1_len, mystery_dungeon_rescue_team_boss_ch2_len, 0},
{pallet_townName, pallet_town_ch1, pallet_town_ch2, pallet_town_ch3, pallet_town_ch1_len, pallet_town_ch2_len, pallet_town_ch3_len},
{pearl_fey_with_pearlyName, pearl_fey_with_pearly_ch1, pearl_fey_with_pearly_ch2, 0, pearl_fey_with_pearly_ch1_len, pearl_fey_with_pearly_ch2_len, 0},
{miror_b_colName, miror_b_col_ch1, miror_b_col_ch2, 0, miror_b_col_ch1_len, miror_b_col_ch2_len, 0},
{miror_b_pokemon_xdName, miror_b_pokemon_xd_ch1, miror_b_pokemon_xd_ch2, 0, miror_b_pokemon_xd_ch1_len, miror_b_pokemon_xd_ch2_len, 0},
{aa1_pursuit_corneredName, aa1_pursuit_cornered_ch1, aa1_pursuit_cornered_ch2, 0, aa1_pursuit_cornered_ch1_len, aa1_pursuit_cornered_ch2_len, 0},
{aa2_pursuit_questionedName, aa2_pursuit_questioned_ch1, aa2_pursuit_questioned_ch2, aa2_pursuit_questioned_ch3, 0, aa2_pursuit_questioned_ch1_len, aa2_pursuit_questioned_ch2_len, aa2_pursuit_questioned_ch3_len},
{quartz_quadrantName, quartz_quadrant_ch1, quartz_quadrant_ch2, 0, quartz_quadrant_ch1_len, quartz_quadrant_ch2_len, 0},
{rby_cerulean_cityName, rby_cerulean_city_ch1, rby_cerulean_city_ch2, rby_cerulean_city_ch3, rby_cerulean_city_ch1_len, rby_cerulean_city_ch2_len, rby_cerulean_city_ch3_len},
{rby_game_cornerName, rby_game_corner_ch1, rby_game_corner_ch2, 0, rby_game_corner_ch1_len, rby_game_corner_ch2_len, 0},
{rby_gym_leaderName, rby_gym_leader_ch1, rby_gym_leader_ch2, 0, rby_gym_leader_ch1_len, rby_gym_leader_ch2_len, 0},
{rby_pokemon_centerName, rby_pokemon_center_ch1, rby_pokemon_center_ch2, 0, rby_pokemon_center_ch1_len, rby_pokemon_center_ch2_len, 0},
{rby_surfingName, rby_surfing_ch1, rby_surfing_ch2, 0, rby_surfing_ch1_len, rby_surfing_ch2_len, 0},
{ss_aqua_advanceName, ss_aqua_advance_ch1, ss_aqua_advance_ch2, 0, ss_aqua_advance_ch1_len, ss_aqua_advance_ch2_len, 0},
{tetrisName, tetris_ch1, tetris_ch2, 0, tetris_ch1_len, tetris_ch2_len, 0},
{field_of_hopes_and_dreamsName, field_of_hopes_and_dreams_ch1, field_of_hopes_and_dreams_ch2, 0, field_of_hopes_and_dreams_ch1_len, field_of_hopes_and_dreams_ch2_len, 0},
{aa3_confess_the_truthName, aa3_confess_the_truth_ch1, aa3_confess_the_truth_ch2, 0, aa3_confess_the_truth_ch1_len, aa3_confess_the_truth_ch2_len, 0},
{aa3_objection2004Name, aa3_objection2004_ch1, aa3_objection2004_ch2, 0, aa3_objection2004_ch1_len, aa3_objection2004_ch2_len, 0},
{aa3_godot_reminiscenceName, aa3_godot_reminiscence_ch1, aa3_godot_reminiscence_ch2, 0, aa3_godot_reminiscence_ch1_len, aa3_godot_reminiscence_ch2_len, 0},
{aa3_godot_mainName, aa3_godot_main_ch1, aa3_godot_main_ch2, 0, aa3_godot_main_ch1_len, aa3_godot_main_ch2_len, 0}
{turnabout_sistersName, turnabout_sisters_ch1, turnabout_sisters_ch2, turnabout_sisters_ch3, turnabout_sisters_ch1_len, turnabout_sisters_ch2_len, turnabout_sisters_ch3_len},

};
