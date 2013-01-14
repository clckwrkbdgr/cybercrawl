
#include "linuxlib.h"

#include <string.h>
#include <stdlib.h>

#include "bang.h"
#include "beam.h"
#include "effects.h"
#include "externs.h"
#include "enum.h"
#include "invent.h"
#include "items.h"
#include "it_use2.h"
#include "it_use3.h"
#include "monplace.h"
#include "monstuff.h"
#include "misc.h"
#include "mutation.h"
#include "player.h"
#include "ouch.h"
#include "religion.h"
#include "spell.h"
#include "spells.h"
#include "spells0.h"
#include "spells1.h"
#include "spells3.h"
#include "stuff.h"
#include "view.h"

int mutate(int which_mutation);


int learned = 0;
int spell_container = 0;
int sc_read_1, sc_read_2;
int i;
int book_thing;
int keyin;


int miscast_effect(int sp_type, int mag_pow, int mag_fail, int force_effect)
{

/*
sp_type is the type of the spell
mag_pow is overall power of the spell or effect (ie its level)
mag_fail is the degree to which you failed
force_effect forces a certain effect to occur. Currently unused.
*/

struct bolt beam [1];

int loopj = 0;
int spec_effect = 0;
int hurted = 0;

spec_effect = mag_pow * mag_fail * (10 + mag_pow) / 7;
if (force_effect == 100 && random2(40) > spec_effect && random2(40) > spec_effect)
{
 goto nothing_happening;
}
spec_effect = spec_effect / 100;

#ifdef DEBUG
int failure = spec_effect;
#endif

spec_effect = random2(spec_effect);
if (spec_effect > 3) spec_effect = 3;
if (spec_effect < 0) spec_effect = 0;

#ifdef DEBUG
msg("Sptype: @1, failure1: @2, failure2: @3") << sp_type << failure << spec_effect;
#endif



if (force_effect != 100) spec_effect = force_effect;


switch(sp_type)
{

case 11: // conjuration
switch(spec_effect)
{
 case 0: // just a harmless message
 switch(random2(10))
 {
  case 0: mpr("Sparks fly from your hands!"); break;
  case 1: mpr("The air around you crackles with energy!"); break;
  case 2: mpr("Wisps of smoke drift from your fingertips."); break;
  case 3: mpr("You feel a strange surge of energy!"); break;
  case 4: mpr("You are momentarily dazzled by a flash of light!"); break;
  case 5: mpr("Strange energies run through your body."); break;
  case 6: mpr("Your skin tingles."); break;
  case 7: mpr("Your skin glows momentarily."); break;
  case 8: mpr("Nothing appears to happen."); break;
  case 9: mpr("You smell something strange."); break;
 }
 break;

 case 1: // a bit less harmless stuff
 switch(random2(2))
 {
  case 0:
  mpr("Smoke pours from your fingertips!");
  big_cloud(5, you[0].x_pos, you[0].y_pos, 20);
  break;
  case 1:
  mpr("A wave of violent energy washes through your body!");
  ouch(2 + random2(3) + random2(3), 0, 18);
  break;
 }
 break;

 case 2: // rather less harmless stuff
 switch(random2(2))
 {
  case 0:
  mpr("You are blasted with cyber energy!");
  ouch(4 + random2(7) + random2(7), 0, 18);
  break;
  case 1:
  mpr("You conjure up a violent explosion!");
  mpr("Oops.");
  beam[0].type = 43;
  beam[0].damage = 107;
  beam[0].flavour = 0; // <=- not sure about this
  beam[0].bx = you[0].x_pos;
  beam[0].by = you[0].y_pos;
  beam[0].beam_name = "explosion";
  beam[0].colour = random2(15) + 1;
  beam[0].thing_thrown = 1; // your explosion (is this right?)
  explosion(0, beam);
  noisy(10, you[0].x_pos, you[0].y_pos);
  break;
 }
 break;

 case 3: // considerably less harmless stuff
 switch(random2(2))
 {
  case 0:
  mpr("Energy rips through your body!");
  ouch(9 + random2(12) + random2(12), 0, 18);
  break;
  case 1:
  mpr("There is a sudden explosion of cyber energy!");
  beam[0].type = 43;
  beam[0].damage = 110;
  beam[0].flavour = 0; // <=- not sure about this
  beam[0].bx = you[0].x_pos;
  beam[0].by = you[0].y_pos;
  beam[0].beam_name = "explosion";
  beam[0].colour = random2(15) + 1;
  beam[0].thing_thrown = 1; // your explosion (is this right?)
  explosion(random2(2), beam);
  noisy(20, you[0].x_pos, you[0].y_pos);
  break;
 }
 break;

}
break; // end conjuration

case 12:
switch(spec_effect)
{
 case 0: // just a harmless message
 switch(random2(10))
 {
  case 0: mpr("Your hands glow momentarily."); break;
  case 1: mpr("The air around you crackles with energy!"); break;
  case 2: mpr("Multicolored lights dance before your eyes!"); break;
  case 3: mpr("You feel a strange surge of energy!"); break;
  case 4: mpr("Waves of light ripple over your body."); break;
  case 5: mpr("Strange energies run through your body."); break;
  case 6: mpr("Your skin tingles."); break;
  case 7: mpr("Your skin glows momentarily."); break;
  case 8: mpr("Nothing appears to happen."); break;
  case 9: mpr("You hear something strange."); break;
 }
 break;

 case 1: // slightly annoying
 switch(random2(2))
 {
  case 0:
  potion_effect(POT_LEVITATION, 20); // levitation
  break;
  case 1:
  random_uselessness(2 + random2(7), 0);
  break;
 }
 break;

 case 2: // much more annoying
 switch(random2(3))
 {
  case 0: // curse
  curse_an_item(0, 0);
  mpr("You sense a malignant aura.");
  break;
  case 1:
  potion_effect(POT_SLOWING, 10); // slow
  break;
  case 2:
  potion_effect(POT_BERSERK_RAGE, 10); // berserk
  break;
 }
 break;

 case 3: // potentially lethal
 switch(random2(4))
 {
  case 0: // curse
  do
  {
   curse_an_item(0, 0);
   loopj = random2(3);
  } while (loopj != 0);
  mpr("You sense an overwhelmingly malignant aura!");
  break;
  case 1:
  potion_effect(POT_PARALYSIS, 10); // paral
  break;
  case 2:
  potion_effect(POT_CONFUSION, 10); // conf
  break;
  case 3:
  if (mutate(100) == 0) mpr("Nothing appears to happen.");
  break;
 }
 break;

}
break; // end enchantments

case 20: // translocations
switch(spec_effect)
{
 case 0: // just a harmless message
 switch(random2(10))
 {
  case 0: mpr("Space warps around you."); break;
  case 1: mpr("The air around you crackles with energy!"); break;
  case 2: mpr("You feel a wrenching sensation."); break;
  case 3: mpr("You feel a strange surge of energy!"); break;
  case 4: mpr("You spin around."); break;
  case 5: mpr("Strange energies run through your body."); break;
  case 6: mpr("Your skin tingles."); break;
  case 7: mpr("The world appears momentarily distorted!"); break;
  case 8: mpr("Nothing appears to happen."); break;
  case 9: mpr("You feel uncomfortable."); break;
 }
 break;

 case 1: // mostly harmless
 switch(random2(3))
 {
  case 0:
  mpr("You create a localised field of spatial distortion.");
  mpr("Ouch!");
  ouch(3 + random2(3) + random2(3), 0, 18);
  break;
  case 1:
  mpr("Space bends around you!");
  random_blink();
  ouch(2 + random2(2) + random2(2), 0, 18);
  break;
  case 2:
  mpr("Space twists in upon itself!");
  create_monster(MONS_SPATIAL_VORTEX, 24, 1, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
  break;
 }
 break;

 case 2: // less harmless
 switch(random2(2))
 {
  case 0:
  mpr("You create a strong localised field of spatial distortion.");
  mpr("Ouch!!");
  ouch(7 + random2(7) + random2(7), 0, 18);
  break;
  case 1:
  mpr("Space warps around you!");
  if (random2(3) == 0) you_teleport2(1);
  	else random_blink();
  ouch(5 + random2(5) + random2(5), 0, 18);
  potion_effect(POT_CONFUSION, 10); // conf
  break;
  case 2:
  mpr("Space twists in upon itself!");
  for (loopj = 0; loopj < 2 + random2(3); loopj ++)
  {
   create_monster(MONS_SPATIAL_VORTEX, 22, 1, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
  }
  break;
 }
 break;

 case 3: // much less harmless
 switch(random2(4))
 {
  case 0:
  mpr("You create an extremely strong localised field of spatial distortion!");
  mpr("Ouch!!!");
  ouch(11 + random2(9) + random2(9), 0, 18);
  break;
  case 1:
  mpr("Space warps crazily around you!");
  you_teleport2(1);
  ouch(7 + random2(7) + random2(7), 0, 18);
  potion_effect(POT_CONFUSION, 30); // conf
  break;
  case 2:
  mpr("You are thrown into the Dump!");
  more();
  banished(96); // sends you to the abyss
  break;
  case 3:
  if (mutate(100) == 0) mpr("Nothing appears to happen.");
  break;
 }
 break;


}
break; // end translocations

case 18: // Summonings
switch(spec_effect)
{
 case 0: // just a harmless message
 switch(random2(10))
 {
  case 0: mpr("Shadowy shapes form in the air around you, then vanish."); break;
  case 1: mpr("You hear strange voices."); break;
  case 2: mpr("Your head hurts."); break;
  case 3: mpr("You feel a strange surge of energy!"); break;
  case 4: mpr("Your brain hurts!"); break;
  case 5: mpr("Strange energies run through your body."); break;
  case 6: mpr("The world appears momentarily distorted."); break;
  case 7: mpr("Space warps around you."); break;
  case 8: mpr("Nothing appears to happen."); break;
  case 9: mpr("Distant voices call out to you!"); break;
 }
 break;

 case 1: // a little bad
 switch(random2(2))
 {
  case 0: // identical to translocation
  mpr("You create a localised field of spatial distortion.");
  mpr("Ouch!");
  ouch(3 + random2(3) + random2(3), 0, 18);
  break;
  case 1:
  mpr("Space twists in upon itself!");
  create_monster(MONS_SPATIAL_VORTEX, 24, 1, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
  break;
 }

 case 2: // more bad
 switch(random2(2))
 {
  case 0:
  mpr("Space twists in upon itself!");
  for (loopj = 0; loopj < 2 + random2(3); loopj ++)
  {
   create_monster(MONS_SPATIAL_VORTEX, 22, 1, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
  }
  break;
  case 1:
  mpr("Something appears in a flash of light!");
  create_monster(MONS_WHITE_IMP + random2(5), 24, 1, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
  break;
 }
 break;


 case 3: // more bad
 switch(random2(4))
 {
  case 0:
  mpr("Something forms out of thin air.");
  create_monster(MONS_LARGE_ABOMINATION, 0, 1, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
  break;
  case 1:
  mpr("You sense a hostile presence.");
  create_monster(MONS_WHITE_IMP + random2(10), 0, 1, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
  break;
  case 2:
  mpr("Something turns its attention towards you...");
  for (loopj = 0; loopj < 2 + random2(3); loopj ++)
  {
   create_monster(MONS_WHITE_IMP + random2(5), 22, 1, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
  }
  break;
  case 3:
  mpr("You are thrown into the Dump!");
  banished(96); // sends you to the abyss
  break;
 }
 break;
// A powerful entity turns its attention onto you

} // end Summonings
break;


case 19: // Divinations
switch(spec_effect)
{
 case 0: // just a harmless message
 switch(random2(10))
 {
  case 0: mpr("Weird images run through your mind."); break;
  case 1: mpr("You hear strange voices."); break;
  case 2: mpr("Your head hurts."); break;
  case 3: mpr("You feel a strange surge of energy!"); break;
  case 4: mpr("Your brain hurts!"); break;
  case 5: mpr("Strange energies run through your body."); break;
  case 6: mpr("Everything looks hazy for a moment."); break;
  case 7: mpr("You seem to have forgotten something, but you can't remember what it was!"); break;
  case 8: mpr("Nothing appears to happen."); break;
  case 9: mpr("You feel uncomfortable."); break;
 }
 break;

 case 1: // more annoying things
 switch(random2(2))
 {
  case 0:
  mpr("You feel slightly disoriented.");
  forget_map(10 + random2(10));
  break;
  case 1:
  potion_effect(POT_CONFUSION, 1); // conf
  break;
 }
 break;

 case 2: // even more annoying things
 switch(random2(2))
 {
  case 0:
  if (you[0].is_undead != 0)
  {
   mpr("You suddenly recall your previous life!");
   break;
  }
  if (player_sust_abil() != 0 || you[0].intel <= 3)
  {
   mpr("You have a terrible headache.");
   break;
  }
  mpr("You have damaged your brain!");
  you[0].intel --;
  you[0].intel_ch = 1;
  potion_effect(POT_CONFUSION, 1); // conf
  break;
  case 1:
  mpr("You feel lost.");
  forget_map(40 + random2(40));
  potion_effect(POT_CONFUSION, 1); // conf
  break;

 }
 break;

 case 3: // nasty
 switch(random2(3))
 {
  case 0:
  if (forget_spell() == 1)
  {
   mpr("Some program have been uninstalled!");
  } else mpr("You get a splitting headache.");
  break;
  case 1:
  mpr("You feel completely lost.");
  forget_map(100);
  potion_effect(POT_CONFUSION, 100); // conf
  break;
  case 2:
  if (you[0].is_undead != 0)
  {
   mpr("You suddenly recall your previous life.");
   break;
  }
  if (player_sust_abil() != 0 || you[0].intel <= 3)
  {
   mpr("You have a terrible headache.");
   break;
  }
  mpr("You have damaged your brain!");
  you[0].intel -= 2 + random2(2);
  if (you[0].intel <= 3) you[0].intel = 3;
  you[0].intel_ch = 1;
  potion_effect(POT_CONFUSION, 100); // conf
  break;
 }
 break;

}
break; // end divinations

 case 16: // necromancy
 if (you[0].religion == GOD_KIKUBAAQUDGHA && you[0].piety >= 50 && random2(150) <= you[0].piety)
 {
  mpr("Nothing appears to happen.");
  break;
 }
 switch(spec_effect)
 {
  case 0:
  switch(random2(10))
  {
   case 0: mpr("You smell decay."); break;
   case 1: mpr("You hear strange and distant voices."); break;
   case 2: mpr("Pain shoots through your body."); break;
   case 3: mpr("Your bones ache."); break;
   case 4: mpr("The world around you seems to dim momentarily."); break;
   case 5: mpr("Strange energies run through your body."); break;
   case 6: mpr("You shiver with cold."); break;
   case 7: mpr("You sense a malignant aura."); break;
   case 8: mpr("Nothing appears to happen."); break;
   case 9: mpr("You feel very uncomfortable."); break;
  }
  break;

  case 1: // a bit nasty
  switch(random2(3))
  {
   case 0:
   if (you[0].is_undead != 0)
   {
    mpr("You feel weird for a moment.");
    break;
   }
   mpr("Pain shoots through your body!");
   ouch(random2(4) + random2(4), 0, 18);
   break;
   case 1:
   mpr("You feel horribly lethargic.");
   potion_effect(POT_SLOWING, 15); // slow
   break;
   case 2:
   mpr("You smell decay."); // identical to a harmless message
   you[0].rotting ++;
   break;
  }
  break;

  case 2: // much nastier
  switch(random2(3))
  {
   case 0:
   mpr("You are surrounded by flickering shadows.");
   create_monster(MONS_SHADOW, 21, 1, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
   break;
   case 1:
   if (random2(3) == 0 && player_prot_life() == 0)
   {
    drain_exp();
    break;
   } // otherwise it just flows through...
   case 2:
   if (you[0].is_undead != 0)
   {
    mpr("You feel weird for a moment.");
    break;
   }
   mpr("You convulse helplessly as pain tears through your body!");
   ouch(random2(12) + random2(12) + 5, 0, 18);
   break;
  }
  break;

  case 3: // even nastier
  switch(random2(5))
  {
   case 0:
   if (you[0].is_undead != 0)
   {
    mpr("Something just walked over your grave. No, really!");
    break;
   }
   mpr("Your body is wracked with pain!");
   loopj = (you[0].hp / 2) - 1;
   if (loopj <= 0) loopj = 0;
   ouch(loopj, 0, 0); // can never die from this, right?
   you[0].hp_ch = 1;
   break;
   case 1:
   mpr("You are engulfed in negative energy!");
   if (player_prot_life() == 0)
   {
    drain_exp();
    break;
   } // otherwise it just flows through...
   case 2:
   lose_stat(100, 1);
   break;
   case 3:
   if (you[0].is_undead != 0)
   {
    mpr("You feel terrible.");
    break;
   }
   mpr("You feel your flesh start to rot away!");
   you[0].rotting += random2(4) + 1;
   break;
   case 4:
   mpr("Something reaches out for you...");
   create_monster(MONS_WRAITH, 23, 1, you[0].x_pos, you[0].y_pos, MHITNOT, 250);
   break;
  }
  break;

 }
 break; // end necromancy

  case 15: /* transmigr */
  switch(spec_effect)
  {
   case 0: // just a harmless message
   switch(random2(10))
   {
    case 0: mpr("Your hands glow momentarily."); break;
    case 1: mpr("The air around you crackles with energy!"); break;
    case 2: mpr("Multicolored lights dance before your eyes!"); break;
    case 3: mpr("You feel a strange surge of energy!"); break;
    case 4: mpr("Waves of light ripple over your body."); break;
    case 5: mpr("Strange energies run through your body."); break;
    case 6: mpr("Your skin tingles."); break;
    case 7: mpr("Your skin glows momentarily."); break;
    case 8: mpr("Nothing appears to happen."); break;
    case 9: mpr("You smell something strange."); break;
   }
   break;

   case 1: // slightly annoying
   switch(random2(2))
   {
    case 0:
    mpr("Your body is twisted painfully.");
    ouch(random2(4) + random2(4), 0, 18);
    break;
    case 1:
    random_uselessness(2 + random2(7), 0);
    break;
   }
   break;

   case 2: // much more annoying
   switch(random2(4))
   {
    case 0:
    mpr("Your body is twisted very painfully!");
    ouch(random2(12) + random2(12), 0, 18);
    break;
    case 1:
    mpr("Strange energies tear through your body!");
    mutate(100);
    break;
    case 2:
    potion_effect(POT_PARALYSIS, 10); // paral
    break;
    case 3:
    potion_effect(POT_CONFUSION, 10); // conf
    break;
   }
   break;

   case 3: // even nastier
   switch(random2(3))
   {
    case 0:
    mpr("Your body is distorted in a weird and horrible way!");
    mutate(100);
    mutate(100);
    mutate(100);
    mutate(100);
    ouch(random2(12) + random2(12), 0, 18);
    break;
    case 1:
    mpr("You feel very strange.");
    delete_mutation(100);
    delete_mutation(100);
    delete_mutation(100);
    delete_mutation(100);
    delete_mutation(100);
    delete_mutation(100);
    delete_mutation(100);
    ouch(random2(12) + random2(12), 0, 18);
    break;
    case 2:
    mpr("Your body is distorted in a weirdly horrible way!");
    if (give_bad_mutation() == 0)
     if (give_bad_mutation() == 0)
      if (give_bad_mutation() == 0)
        give_bad_mutation();
    ouch(random2(12) + random2(12), 0, 18);
    break;
   }
 break;

}
break; // end enchantments



case 13: // fire
switch(spec_effect)
{
 case 0: // just a harmless message
 switch(random2(10))
 {
  case 0: mpr("Sparks fly from your hands!"); break;
  case 1: mpr("The air around you burns with energy!"); break;
  case 2: mpr("Wisps of smoke drift from your fingertips."); break;
  case 3: mpr("You feel a strange surge of energy!"); break;
  case 4: mpr("You smell smoke."); break;
  case 5: mpr("Heat runs through your body."); break;
  case 6: mpr("You feel uncomfortably hot."); break;
  case 7: mpr("Lukewarm flames ripple over your body."); break;
  case 8: mpr("Nothing appears to happen."); break;
  case 9: mpr("You hear a sizzling sound."); break;
 }
 break;

 case 1: // a bit less harmless stuff
 switch(random2(2))
 {
  case 0:
  mpr("Smoke pours from your fingertips!");
  big_cloud(5 + random2(3), you[0].x_pos, you[0].y_pos, 20);
  break;
  case 1:
  mpr("Flames dance in the air around you.");
  scrolls_burn(3, 6);
  if (player_res_fire() < 100) ouch(2 + random2(3) + random2(3), 0, 18);
  break;
 }
 break;

 case 2: // rather less harmless stuff
 switch(random2(2))
 {
  case 0:
  mpr("You are blasted with flames.");
  ouch(check_your_resists(3 + random2(7) + random2(7), 2), 0, 18);
  scrolls_burn(5, 6);
  break;
  case 1:
  mpr("You conjure up a fiery explosion!");
  mpr("Oops.");
  beam[0].type = 43;
  beam[0].damage = 107;
  beam[0].flavour = 2;
  beam[0].bx = you[0].x_pos;
  beam[0].by = you[0].y_pos;
  beam[0].beam_name = "explosion";
  beam[0].colour = RED;
  beam[0].thing_thrown = 1; // your explosion (is this right?)
  explosion(0, beam);
  noisy(10, you[0].x_pos, you[0].y_pos);
  break;
 }
 break;

 case 3: // considerably less harmless stuff
 switch(random2(3))
 {
  case 0:
  mpr("You are blasted with searing flames!");
  ouch(check_your_resists(8 + random2(12) + random2(12), 2), 0, 18);
  scrolls_burn(10, 6);
  break;
  case 1:
  mpr("There is a sudden and violent explosion of flames!");
  beam[0].type = 43;
  beam[0].damage = 110;
  beam[0].flavour = 2;
  beam[0].bx = you[0].x_pos;
  beam[0].by = you[0].y_pos;
  beam[0].beam_name = "fireball";
  beam[0].colour = RED;
  beam[0].thing_thrown = 1; // your explosion (is this right?)
  explosion(random2(2), beam);
  noisy(20, you[0].x_pos, you[0].y_pos);
  break;
  case 2:
  mpr("You are covered in liquid fire!");
  you[0].duration [DUR_LIQUID_FLAMES] += 1 + random2(3) + random2(3) + random2(3);
  break;
 }
 break;
}
break; // end fire


case 14: // ice
switch(spec_effect)
{
 case 0: // just a harmless message
 switch(random2(10))
 {
  case 0: mpr("You shiver with cold."); break;
  case 1: mpr("A chill runs through your body."); break;
  case 2: mpr("Wisps of condensation drift from your fingertips."); break;
  case 3: mpr("You feel a strange surge of energy!"); break;
  case 4: mpr("Your hands feel numb with cold."); break;
  case 5: mpr("A chill runs through your body."); break;
  case 6: mpr("You feel uncomfortably cold."); break;
  case 7: mpr("Frost covers your body."); break;
  case 8: mpr("Nothing appears to happen."); break;
  case 9: mpr("You hear a crackling sound."); break;
 }
 break;

 case 1: // a bit less harmless stuff
 switch(random2(2))
 {
  case 0:
  mpr("You feel extremely cold.");
  break;
  case 1:
  mpr("You are covered in a thin layer of ice");
  scrolls_burn(2, 8);
  if (player_res_cold() < 100) ouch(2 + random2(3) + random2(3), 0, 18);
  break;
 }
 break;

 case 2: // rather less harmless stuff
 switch(random2(2))
 {
  case 0:
  mpr("Heat is drained from your body.");
  ouch(check_your_resists(3 + random2(4) + random2(4), 3), 0, 18);
  scrolls_burn(4, 8);
  break;
  case 1:
  mpr("You conjure up an explosion of ice and frost!");
  mpr("Oops.");
  beam[0].type = 43;
  beam[0].damage = 107;
  beam[0].flavour = 3;
  beam[0].bx = you[0].x_pos;
  beam[0].by = you[0].y_pos;
  beam[0].beam_name = "explosion";
  beam[0].colour = WHITE;
  beam[0].thing_thrown = 1; // your explosion (is this right?)
  explosion(0, beam);
  noisy(10, you[0].x_pos, you[0].y_pos);
  break;
 }
 break;

 case 3: // less harmless stuff
 switch(random2(2))
 {
  case 0:
  mpr("You are blasted with ice!");
  ouch(check_your_resists(5 + random2(6) + random2(6), 3), 0, 18);
  scrolls_burn(9, 8);
  break;
  case 1:
  mpr("Freezing gasses pour from your hands!");
  big_cloud(3, you[0].x_pos, you[0].y_pos, 20);
  break;
 }
 break;

}
break; // end ice

case 22: // Earth
switch(spec_effect)
{
 case 0: // just a harmless message
 case 1:
 switch(random2(10))
 {
  case 0: mpr("You feel earthy."); break;
  case 1: mpr("You are showered with tiny particles of grit."); break;
  case 2: mpr("Sand pours from your fingers."); break;
  case 3: mpr("You feel a strange surge of energy!"); break;
  case 4: mpr("You sympathise with the stones."); break;
  case 5: mpr("You feel gritty."); break;
  case 6: mpr("You feel like a piece of rock."); break;
  case 7: mpr("You feel like a paving stone."); break;
  case 8: mpr("Nothing appears to happen."); break;
  case 9: mpr("You feel chalky."); break;
 }
 break;

 case 2: // slightly less harmless stuff
 switch(random2(1))
 {
  case 0:
  switch(random2(3))
  {
   case 0: mpr("You are hit by flying rocks!"); break;
   case 1: mpr("You are blasted with sand!"); break;
   case 2: mpr("Rocks fall onto you out of nowhere!"); break;
  }
  hurted = 7 + random2(7) + random2(7);
  if (player_AC() > 0) hurted -= random2(player_AC());
  ouch(hurted, 0, 18);
  break;
 }
 break;

 case 3: // less harmless stuff
 switch(random2(1))
 {
  case 0:
  mpr("You conjure up an explosion of flying shrapnel!");
  mpr("Oops.");
  beam[0].type = 43;
  beam[0].damage = 108;
  beam[0].flavour = 19; // shrapnel
  beam[0].bx = you[0].x_pos;
  beam[0].by = you[0].y_pos;
  beam[0].beam_name = "explosion";
  beam[0].colour = CYAN;
  if (random2(5) == 0) beam[0].colour = BROWN;
  if (random2(5) == 0) beam[0].colour = LIGHTCYAN;
  beam[0].thing_thrown = 1; // your explosion (is this right?)
  explosion(0, beam);
  noisy(10, you[0].x_pos, you[0].y_pos);
  break;
 }
 break;

}
break; // end Earth

case 23: // air
switch(spec_effect)
{
 case 0: // just a harmless message
 switch(random2(10))
 {
  case 0: mpr("Ouch! You gave yourself an electric shock."); break;
  case 1: mpr("You feel momentarily weightless."); break;
  case 2: mpr("Wisps of vapour drift from your fingertips."); break;
  case 3: mpr("You feel a strange surge of energy!"); break;
  case 4: mpr("You feel electric!"); break;
  case 5: mpr("Sparks of electricity dance on your fingertips."); break;
  case 6: mpr("You are blasted with air!"); break;
  case 7: mpr("You hear a whooshing sound."); break;
  case 8: mpr("Nothing appears to happen."); break;
  case 9: mpr("You hear a crackling sound."); break;
 }
 break;

 case 1: // a bit less harmless stuff
 switch(random2(2))
 {
  case 0:
  mpr("There is a shower of sparks.");
  break;
  case 1:
  mpr("The wind howls around you!");
  break;
 }
 break;

 case 2: // rather less harmless stuff
 switch(random2(2))
 {
  case 0:
  mpr("Electricity courses through your body.");
  ouch(check_your_resists(3 + random2(4) + random2(4), 5), 0, 18);
  break;
  case 1:
  mpr("Noxious gasses pour from your hands!");
  big_cloud(2, you[0].x_pos, you[0].y_pos, 20);
  break;
 }
 break;

 case 3: // less harmless stuff
 switch(random2(2))
 {
  case 0:
  mpr("You conjure up an explosion of electrical discharges!");
  mpr("Oops.");
  beam[0].type = 43;
  beam[0].damage = 105;
  beam[0].flavour = 5;
  beam[0].bx = you[0].x_pos;
  beam[0].by = you[0].y_pos;
  beam[0].beam_name = "explosion";
  beam[0].colour = LIGHTBLUE;
  beam[0].thing_thrown = 1; // your explosion (is this right?)
  explosion(0 + (random2(4) != 0), beam);
  noisy(10, you[0].x_pos, you[0].y_pos);
  break;
  case 1:
  mpr("Venomous gasses pour from your hands!");
  big_cloud(4, you[0].x_pos, you[0].y_pos, 20);
  break;
 }
 break;

}
break; // end air


case 21: // poison
switch(spec_effect)
{
 case 0: // just a harmless message
 switch(random2(10))
 {
  case 0: mpr("You feel mildly nauseous."); break;
  case 1: mpr("You feel slightly ill."); break;
  case 2: mpr("Wisps of poison gas drift from your fingertips."); break;
  case 3: mpr("You feel a strange surge of energy!"); break;
  case 4: mpr("You feel faint for a moment."); break;
  case 5: mpr("You feel sick."); break;
  case 6: mpr("You feel odd."); break;
  case 7: mpr("You feel weak for a moment."); break;
  case 8: mpr("Nothing appears to happen."); break;
  case 9: mpr("You hear a slurping sound."); break;
 }
 break;

 case 1: // a bit less harmless stuff
 switch(random2(2))
 {
  case 0:
  if (player_res_poison() != 0) goto nothing_happening;
  msg("You feel sick.");
  you[0].poison += 2 + random2(3);
  break;
  case 1:
  mpr("Noxious gasses pour from your hands!");
  place_cloud(2, you[0].x_pos, you[0].y_pos, 2 + random2(4));
  break;
 }
 break;

 case 2: // rather less harmless stuff
 switch(random2(2))
 {
  case 0:
  if (player_res_poison() != 0) goto nothing_happening;
  msg("You feel very sick.");
  you[0].poison += 3 + random2(5) + random2(5);
  break;
  case 1:
  mpr("Noxious gasses pour from your hands!");
  big_cloud(2, you[0].x_pos, you[0].y_pos, 20);
  break;
 }
 break;

 case 3: // less harmless stuff
 switch(random2(3))
 {
  case 0:
  if (player_res_poison() != 0) goto nothing_happening;
  msg("You feel incredibly sick.");
  you[0].poison += 10 + random2(10) + random2(10);
  break;
  case 1:
  mpr("Venomous gasses pour from your hands!");
  big_cloud(4, you[0].x_pos, you[0].y_pos, 20);
  break;
  case 2:
  if (player_res_poison() != 0) goto nothing_happening;
  lose_stat(100, 1);
  break;
 }
 break;

}
break; // end poison


/*
 11 = conjuration
 12 = enchantment
 13 = fire
 14 = ice
 15 = transmigration
 16 = necromancy
 17 = holy
 18 = summoning
 19 = divination
 20 = translocation
21 = poison
22 = earth
23 = air
*/
//}
//}
}
//break;

//}

return 1;

nothing_happening :
mpr("Nothing appears to happen.");

return 0;

}




#include <bitset>

int learn_a_spell(int splbook, int bitty)
{
	int found_a_one = 0;
	bitty ++;

	std::string strungy = std::bitset<16>(you[0].inv_plus [splbook]).to_string();
	int kl = 0;
	for(int h = 1; h < 7; h ++) {
		if (strungy [h] == '1') kl ++;
		if (kl == bitty) {
			learned = h;
			goto succes;
		}
	}
	return 0;

succes:
	found_a_one = 0;
	int icky = 0;
	icky = strungy.size();
	int numbo = 0;
	int multip = 1;

	for(int j = icky; j >= 1; j --) {
		if (strungy [j] != '1' && strungy [j] != '0') continue;
		if (strungy [j] == '0') {
			multip *= 2;
			continue;
		}
		if (strungy [j] == '1') {
			numbo += multip;
			multip *= 2;
		}
	}

	you[0].inv_plus [splbook] = numbo + 64;
	return 1;
}


int which_spellbook(void)
{
int nthing = 0;

if (you[0].spell_levels <= 0)
{
 mpr("You can't install any more programs yet.");
 return 0;
}

if (you[0].inv_no == 0)
	{
	mpr("You aren't carrying anything.");
	return 0;
	}

query :
msg("You can install @1 more slot@2 of programs.") << you[0].spell_levels << ((you[0].spell_levels == 1) ? "" : "s");

keyin = get_ch();
if (keyin == '?' || keyin == '*')
{
	if (keyin == '?') nthing = get_invent(10);
	if (keyin == '*') nthing = get_invent(-1);
	if ((nthing >= 65 && nthing <= 90) || (nthing >= 97 && nthing <= 122))
	{
		keyin = nthing;
	} else
	{
       mesclr();
		goto query;
	}

}

sc_read_1 = (int) keyin;

if (sc_read_1 < 65 || (sc_read_1 > 90 && sc_read_1 < 97) || sc_read_1 > 122)
{
	mpr("You don't have any such object.");
	return 0;
}

sc_read_2 = conv_lett(sc_read_1);

if (you[0].inv_quant [sc_read_2] == 0)
{
	mpr("You don't have any such object.");
	return 0;
}

if (you[0].inv_class [sc_read_2] != 10)
{
	mpr("That isn't a program archive!");
	return 0;
}

if (you[0].inv_type [sc_read_2] == 41) /* manuals */
{
	mpr("That isn't a program archive!");
	return 0;
}

if (you[0].inv_type [sc_read_2] == 35) /* tome of destr */
{
  tome_of_power(sc_read_2);
  return 0;
}

spell_container = sc_read_2;

read_book(spell_container);

if (you[0].inv_plus [spell_container] == 64)
{
   mpr("That archive is empty.");
   return 0;
}

clrscr();
/*scrloc = 0;*/
return 1;

}

void read_book(int book_read)
{
int key2 = 0;

/* remember that this function is called from staff spells as well. */
if (you[0].inv_class [book_read] == 11) key2 = spellbook_contents(you[0].inv_plus [book_read], you[0].inv_type [book_read] + 40);
 else key2 = spellbook_contents(you[0].inv_plus [book_read], you[0].inv_type [book_read]);

if (you[0].inv_class [book_read] == 10)
{
 you[0].had_item [you[0].inv_type [book_read]] = 1;
 if (you[0].inv_type [book_read] == 0 || you[0].inv_type [book_read] == 1 || you[0].inv_type [book_read] == 2)
 {
  you[0].had_item [0] = 1;
  you[0].had_item [1] = 1;
  you[0].had_item [2] = 1;
 }
 if (you[0].inv_type [book_read] == 3 || you[0].inv_type [book_read] == 4)
 {
  you[0].had_item [3] = 1;
  you[0].had_item [4] = 1;
 }
}

redraw_screen();
/* Put special book effects in another function which can be called from
   memorise as well */
you[0].turnover = 1;
you[0].inv_ident [book_read] = 1;
book_thing = key2;
keyin = key2;         // FIX this should probably go...

return;

}


void which_spell(void)
{
int chance = 0;
int powm;
int levels_needed = 0;


int i;
int j = 0;

for (i = SK_SPELLCASTING; i < SK_POISON_MAGIC; i ++)
{
 if (you[0].skills [i] != 0) j ++;
}

if (j == 0)
{
 mpr("You can't use programs! I'm afraid it's devices only for now.");
 return;
}

if (which_spellbook() == 0) return;

sc_read_1 = (int) book_thing;

if (sc_read_1 < 65 || (sc_read_1 > 90 && sc_read_1 < 97) || sc_read_1 > 122)
{
	whatt :
        redraw_screen();
		mpr("What?");
	return;
}


sc_read_2 = conv_lett(sc_read_1);


if (sc_read_2 > 7)
{
   goto whatt;
}


if (learn_a_spell(spell_container, sc_read_2) != 1)
{
   goto whatt;
}



int specspell = which_spell_in_book(you[0].inv_type [spell_container], learned);
int is_good = 1;


if (you[0].spell_no == 21 && specspell != 43) //if changed, must also change for priest in level_change. You can always memorise selective amnesia
{
 mpr("Your cyberbrain is already too full of programs!");
 return;
}

if (you[0].species == SP_MUMMY && spell_type(specspell, 17) == 1)
{
  mpr("You can't use this type of programs!");
  return;
}

//if (you[0].is_undead != 0 && specspell == 42)
if ((you[0].is_undead == 1 && undead_can_memorise(specspell) == 2) || (you[0].is_undead == 2 && undead_can_memorise(specspell) != 0))
{
  mpr("You can't use this program.");
  return;
}

for (i = 0; i < 25; i ++)
{
	if (you[0].spells [i] == specspell)
   {
      redraw_screen();
      mpr("You already have that program!");
      you[0].turnover = 1;
      return;
   }
}

levels_needed = spell_value(specspell);

if (you[0].spell_levels < levels_needed)
{
   too_high :
   redraw_screen();
   mpr("You can't install that many programs yet!");
   you[0].turnover = 1;
   return;

}


if (you[0].xl < spell_value(specspell) || (is_good == 0 && you[0].xl < spell_value(specspell) * 2))
 goto too_high;
/*
types of spells:
11 = conjuration
12 = enchantment
13 = fire
14 = ice
15 = transmigration
16 = necromancy
17 = holy
18 = summoning
19 = divination
20 = translocation
21 = you[0].poison
*/


chance = 0;

powm = spell_spec(specspell, 0);

chance = spell_fail(specspell);

 if (chance >= 80)
 {
  mpr("This program is very difficult to install.");
 } else
 if (chance >= 60)
 {
  mpr("This program is quite difficult to commit to memory.");
 } else
 if (chance >= 45)
 {
  mpr("This program is rather tricky to install.");
 } else
 if (chance >= 30)
 {
  mpr("This program is a little tricky to copy.");
 }

redraw_screen();

msg("Install @1?") << spell_name(specspell);
keyin = getkey();

if (keyin == 0) getkey();
if (keyin != 'y' && keyin != 'Y')
{
redraw_screen();
 return;
}
mesclr();

if (you[0].mutation [MUT_BLURRY_VISION] > 0 && random2(4) < you[0].mutation [MUT_BLURRY_VISION])
{
 mpr("The writing blurs into unreadable gibberish.");
 you[0].turnover = 1;
 return;
}

if (random2(40) + random2(40) + random2(40) < chance) //powm <= random2(chance) && spell_value(specspell) > 3)
{
redraw_screen();
 mpr("You fail to install the program.");
 you[0].turnover = 1;
 if (you[0].inv_type [spell_container] == 24)
 {
   mpr("The screen of the Necronomicon glow with a malevolent light...");
   miscast_effect(16, 8, random2(30) + random2(30) + random2(30), 100);
 }
 if (you[0].inv_type [spell_container] == 27) /* Demonology */
 {
   mpr("This archive does not appreciate being disturbed by one of your ineptitude!");
   miscast_effect(18, 7, random2(30) + random2(30) + random2(30), 100);
 }
 if (you[0].inv_type [spell_container] == 33) /* Annihilations */
 {
   mpr("This archive does not appreciate being disturbed by one of your ineptitude!");
   miscast_effect(11, 8, random2(30) + random2(30) + random2(30), 100);
 }

#ifndef DEBUG
 return;
#endif

#ifdef DEBUG
 mpr("But I'll let you install it anyway, okay?");
#endif

}


for (i = 0; i < 25; i ++)
{
	if (you[0].spells [i] == 210) break;
}


you[0].spells [i] = specspell;



you[0].spell_levels -= levels_needed;
you[0].spell_no ++;

you[0].delay_t = spell_value(you[0].spells [i]);
you[0].delay_doing = 3;
//scruggle = 0;

you[0].turnover = 1;
redraw_screen();

naughty(9, 2 + random2(5));

}
