/*    This file includes the scores for the entire 
 * <<Spider Dance>> music based on 
 * https://musescore.com/user/4992161/scores/1342436
 *    The music is in 2/4;
 * 
 * 
 *    There are 86 measures in total. Most of the 
 * measures contains parts for BuzzV1(V1), BuzzV2(V2),
 * and BuzzB(B). If parts are missing, they are repeated parts from
 * other measures or they are buzzers at rest.
 * 
 * 
 *    The formats for declarating melody array and NL array are: 
 *    
 * int melody_buzzer_measureNumber(firstHalf or secondHalf)[] 
 * 
 *                    = {notes}; // corresponding noteLength(NL) array
 * int noteLength#[] = {}; // size
 *                    
 *                    
 *    You can determine the NL arrays as follows,
 * Each measure is divided into two parts, partA(firstHalf) and partB(secondHalf).
 * partA is further divided into 16 sub parts. By regroup these 16 sub parts, you
 * are allowed to create different note lengths for each notes in the measure.
 * Remember, the sum of all elements in a NL array must be 16.
 * 
 *    Each element in melody array should correspond with only one element in 
 * the NL array. (Size of melody array) = (size of NL array).
 * 
 * 
 *    For example,
 *    
 *    int melody_V1_1a[] = {F5, C5}; // NL1
 *    int noteLength1[] = {8, 8}; // 2
 *    
 *    This is the melody for the first half(part a) of the first measure
 * for BuzzerV1. According to the original score, both F5 and C5 are 
 * 8th notes, so they have the same length and each own half of the 
 * 16 sub parts which is 8 (16 / 2 = 8);
 * 
 * 
 *    For reference,
 * quarter note = 16  (no tail)
 *     8th note = 8   (one tail)
 *    16th note = 4   (two tails)
 *    32th note = 2   (three tails)
 *    
 *    
 *    You might need to learn some music theory for determining the length
 * of each note. Or you can search each note length on google and match
 * their look with the original score. 
 * 
 * 
 *    0 in the melody arrays means rest.
 */


int noteLength0[] = {16}; // 1
int noteLength1[] = {8, 8}; // 2
int noteLength2[] = {4, 4, 4, 4}; // 4
int noteLength3[] = {4, 4, 8}; // 3
int noteLength4[] = {4, 4, 2, 2, 4}; // 5
int noteLength5[] = {8, 4, 4}; // 3
int noteLength6[] = {4, 8, 4}; // 3
int noteLength7[] = {4, 2, 2, 8}; // 4
int noteLength8[] = {8, 2, 2, 2, 2}; // 5
int noteLength9[] = {2, 2, 2, 2, 2, 2, 2, 2}; // 8
int noteLength10[] = {8, 4, 2, 2}; // 4



int melody0[] = {0}; // NL0


// M1
int melody_V1_1a[] = {F5, C5}; // NL1
int melody_V1_1b[] = {Ab4, F4}; // NL1
int melody_B_1a[] = {F3, F3, C4, F3}; // NL2
int melody_B_1b[] = {C4, C4, Eb3, Eb3}; // NL2
/* V2 empty */
// M2
int melody_V1_2a[] = {0, B4, Bb4}; // NL3
int melody_V1_2b[] = {Bb4, Ab4, E4, F4}; // NL2
int melody_B_2a[] = {Cs3, Cs3, Cs3, Cs3}; // // NL2
int melody_B_2b[] = {C3, C3, C3, C3}; // // NL2
/* V2 empty */
// M3
int melody_V1_3a[] = {0, C5, Bb4, Ab4}; // NL2
int melody_V1_3b[] = {Bb4, C5, E4, F4}; // NL2
int melody_B_3a[] = {F3, F3, F3, F3}; //NL2
int melody_B_3b[] = {G3, G3, G3, G3}; //NL2
/* V2 empty */
// M4
int melody_V1_4a[] = {Ab4, F4, E4, F5}; // NL2
int melody_V1_4b[] = {0, Eb5, C5, Bb4, Ab4}; // NL4
int melody_B_4a[] = {Ab3, Ab3, Ab3, Ab3}; // NL2
int melody_B_4b[] = {G3, G3, G3, G3}; // NL2
/* V2 empty */
// M5
int melody_V1_5a[] = {F4, Cs4}; // NL1
int melody_V1_5b[] = {G4, Cs4}; // NL1
int melody_B_5a[] = {Cs3, Cs3, Bb3, Cs3}; // NL2
int melody_B_5b[] = {Cs3, Cs3, Bb3, Cs3}; // NL2
/* V2 empty */


// M6
int melody_V1_6a[] = {Ab4, Cs4}; // NL1
int melody_V1_6b[] = {Bb4, Cs4}; // NL1
int melody_B_6a[] = {Cs3, Cs3, F3, Cs3}; // NL2
int melody_B_6b[] = {Cs3, Cs3, Ab3, Cs3}; // NL2
/* V2 empty */
// M7
int melody_V1_7a[] = {C5, Bb4}; // NL1
int melody_V1_7b[] = {F5, C5}; // NL1
int melody_B_7a[] = {C3, 0}; // NL1
int melody_B_7b[] = {0, C3}; // NL1
/* V2 empty */
// M8
int melody_V1_8a[] = {E5, Db5}; // NL1
int melody_V1_8b[] = {C5, 0}; // NL1
int melody_B_8a[] = {C3, 0, C3}; // NL5
int melody_B_8b[] = {C3, C3, D3, E3}; // NL2
/* V2 empty */
// M9
int melody_V1_9a[] = {F5, C6, Bb5, C6}; // NL2
int melody_V1_9b[] = {F5, C6, Bb5, C6}; // NL2
int melody_V2_9a[] = {F4, C4}; // NL1
int melody_V2_9b[] = {Bb3, F3}; // NL1
int melody_B_9a[] = {F3, C3}; // NL1
int melody_B_9b[] = {Bb2, F2}; // NL1
// M10
int melody_V1_10a[] = {Db5, B5, As5, B5}; // NL2
int melody_V1_10b[] = {Eb5, C6, G5, Bb5}; // NL2
int melody_V2_10a[] = {0, B3, Bb3}; // NL3
int melody_V2_10b[] = {Bb3, Gs3, E3, F3}; // NL2
int melody_B_10a[] = {0, B2, Bb2}; // NL3
int melody_B_10b[] = {Bb2, Gs2, E2, F2}; // NL2


// M11
int melody_V1_11a[] = {F5, C6, Bb5, C6}; // NL2
int melody_V1_11b[] = {G5, C6, Bb5, C6}; // NL2
int melody_V2_11a[] = {0, C4, Bb3, Ab3}; // NL2
int melody_V2_11b[] = {Bb3, C4, E3, F3}; // NL2
int melody_B_11a[] = {0, C3, Bb2, Ab2}; // NL2
int melody_B_11b[] = {Bb2, C3, E2, F2}; // NL2
// M12
int melody_V1_12a[] = {Gs5, C6, Bb5, C6}; // NL2
int melody_V1_12b[] = {As5, Db6, C6, Db6}; // NL2
int melody_V2_12a[] = {Ab3, F3, E3, F4}; // NL2
int melody_V2_12b[] = {0, Eb4, C4, Bb3, Ab3}; // NL4
int melody_B_12a[] = {Ab2, F2, E2, F3}; // NL2
int melody_B_12b[] = {0, Eb3, C3, Bb2, Ab2}; // NL4
// M13
int melody_V1_13a[] = {Db5, Ab5, G5, Ab5}; // NL2
int melody_V1_13b[] = {Eb5, Bb5, Ab5, Bb5}; // NL2
int melody_V2_13a[] = {F3, 0}; // NL1
int melody_V2_13b[] = {G3, 0}; // NL1
int melody_B_13a[] = {F2, 0}; // NL1
int melody_B_13b[] = {G2, 0}; // NL1
// M14
int melody_V1_14a[] = {Eb5, C6, Bb5, C6}; // NL2
int melody_V1_14b[] = {Eb5, Db6, C6, Db6}; // NL2
int melody_V2_14a[] = {Ab3, 0}; // NL1
int melody_V2_14b[] = {Bb3, 0}; // NL1
int melody_B_14a[] = {Ab2, 0}; // NL1
int melody_B_14b[] = {Bb2, 0}; // NL1
// M15
int melody_V1_15a[] = {C5, Bb4}; // NL1
int melody_V1_15b[] = {F5, C5}; // NL1
int melody_B_15a[] = {C3, Bb2}; // NL1
int melody_B_15b[] = {F3, C3}; // NL1
/* V2 empty */


// M16
int melody_V1_16a[] = {E5, Db5}; // NL1
int melody_V1_16b[] = {C5}; // NL0
int melody_B_16a[] = {E3, Db3}; // NL1
int melody_B_16b[] = {C3, C3, D3, E3}; // NL2
/* V2 empty */
// M17
int melody_V1_17a[] = {C6, Bb5}; // NL1
int melody_V1_17b[] = {F6, C6}; // NL1
int melody_B_17a[] = {F2, C3}; // NL1
int melody_B_17b[] = {F3, C3}; // NL1
/* V2 empty */
// M18
int melody_V1_18a[] = {E6, Db6}; // NL1
int melody_V1_18b[] = {C6, Db6, C6, Db6}; // NL2
int melody_B_18a[] = {E3, Db3}; // NL1
int melody_B_18b[] = {C3, Db3, C3, Db3}; // NL2
/* V2 empty */
// M19
int melody_V1_19a[] = {C6, A5}; // NL1
int melody_V1_19b[] = {F5, C5}; // NL1
int melody_V2_19a[] = {0, C3, 0, C3}; // NL2
int melody_V2_19b[] = {0, C3, 0, C3}; // NL2
int melody_B_19a[] = {F2, 0, Db3, 0}; // NL2
int melody_B_19b[] = {F2, 0, F3, 0}; // NL2
// M20
int melody_V1_20a[] = {Ab5, F5, G5}; // NL5
int melody_V1_20b[] = {G5, 0}; // NL1
int melody_V2_20a[] = {0, F3, 0, F3}; // NL2
int melody_V2_20b[] = {0, F3, 0, F3}; // NL2
int melody_B_20a[] = {Bb2, 0, Ab3, 0}; // NL2
int melody_B_20b[] = {Bb2, 0, G3, 0}; // NL2


// M21
int melody_V1_21a[] = {Ab5, F5, G5}; // NL5
int melody_V1_21b[] = {G5, 0}; // NL1
int melody_V2_21a[] = {0, F3, 0, F3}; // NL2
int melody_V2_21b[] = {0, F3, 0, F3}; // NL2
int melody_B_21a[] = {Db3, 0, Ab3, 0}; // NL2
int melody_B_21b[] = {Db3, 0, G3, 0}; // NL2
// M22
int melody_V1_22a[] = {Ab5, F5, G5}; // NL5
int melody_V1_22b[] = {G5, Db6, C6, Db6}; // NL2
int melody_V2_22a[] = {0, F3, 0, F3}; // NL2
int melody_V2_22b[] = {0, F3, 0, F3}; // NL2
int melody_B_22a[] = {C3, 0, Ab3, 0}; // NL2
int melody_B_22b[] = {C3, 0, G3, 0}; // NL2
// M23
/* repeat M19 */
// M24
int melody_V1_24a[] = {Ab5, Bb5, G5}; // NL5
int melody_V1_24b[] = {G5, 0}; // NL1
/* repeat M20_V2 & B */
// M25
/* repeat M24_V1,
          M21_V2 & B */


// M26
int melody_V1_26a[] = {Ab5, Bb5, G5}; // NL5
int melody_V1_26b[] = {G5, Db6, C6, Db6}; // NL2
/* repeat M22_V2 & B */
// M27
/* repeat M19 */
// M28
/* repeat M20 */
// M29
/* repeat M21 */
// M30
/* repeat M22 */


// M31
int melody_V1_31a[] = {C6, F6}; // NL1
int melody_V1_31b[] = {Bb5, Ab5}; // NL1
/* repeat M19_V2 & B */
// M32
int melody_V1_32a[] = {Bb5, Ab5, Bb5}; // NL6
int melody_V1_32b[] = {Bb5, Ab5, Bb5}; // NL5
int melody_V2_32a[] = {0, Cs3, 0, Cs3}; // NL2
int melody_V2_32b[] = {0, Cs3, 0, Cs3}; // NL2
int melody_B_32a[] = {Gb2, 0, D3, 0}; // NL2
int melody_B_32b[] = {Gb2, 0, D3, 0}; // NL2
// M33
int melody_V1_33a[] = {B5, Bb5, Ab5}; // NL3
int melody_V1_33b[] = {E5, C5}; // NL1
int melody_V2_33a[] = {0, F3, 0, F3}; // NL2
int melody_V2_33b[] = {0, E3, 0, E3}; // NL2
int melody_B_33a[] = {Db3, 0, Ab3, 0}; // NL2
int melody_B_33b[] = {C3, 0, Gb3, 0}; // NL2
// M34
int melody_V1_34a[] = {Db5, 0, E5, 0}; // NL2
int melody_V1_34b[] = {F5}; // NL0
int melody_V2_34a[] = {0, Ab3, 0, G3}; // NL2
int melody_V2_34b[] = {0}; // NL0
int melody_B_34a[] = {Db3, 0, C3, 0}; // NL2
int melody_B_34b[] = {F3}; // NL0
// M35
int melody_V1_35a[] = {C6, Ab5}; // NL1
int melody_V1_35b[] = {F5, Bb4, C5}; // NL5
/* repeat M19_V2 & B */


// M36
int melody_V1_36a[] = {Ab5, F5}; // NL1
int melody_V1_36b[] = {Db5, Ab4}; // NL1
/* repeat M20_V2 & B */
// M37
int melody_V1_37a[] = {Db5, Ab4, Ab5, Ab4}; // NL2
int melody_V1_37b[] = {G5, Ab5, F5}; // NL3
/* repeat M21_V2 & B */
// M38
int melody_V1_38a[] = {E5, F4, E5, F5}; // NL2
int melody_V1_38b[] = {G5, E5, F5, G5}; // NL2
/* repeat M22_V2 & B */
// M39
int melody_V1_39a[] = {F5, C5, F5, G5}; // NL2
int melody_V1_39b[] = {Ab5, G5, Ab5, Bb5}; // NL2
/* repeat M19_V2 & B */
// M40
int melody_V1_40a[] = {C6, Bb5, Ab5, G5}; // NL2
int melody_V1_40b[] = {Ab5, G5, F5, G5}; // NL2
/* repeat M20_V2 & B */


// M41
int melody_V1_41a[] = {F5, Gs5, Eb6, F5}; // NL2
int melody_V1_41b[] = {Db6, Eb6, C6, Db6}; // NL2
/* repeat M21_V2 & B */
// M42
int melody_V1_42a[] = {Bb5, C6, Ab5, Bb5}; // NL2
int melody_V1_42b[] = {C6}; // NL0
/* repeat M22_V2 & B */
// M43
/* repeat M19 */
// M44
/* repeat M20 */
// M45
/* repeat M21 */


// M46
/* repeat M22 */
// M47
int melody_V1_47a[] = {C6, F6}; // NL1
int melody_V1_47b[] = {Bb5, Ab5}; // NL1
int melody_V2_47a[] = {0, C3, 0, C3}; // NL2
int melody_V2_47b[] = {0, C3, 0, C3}; // NL2
int melody_B_47a[] = {F3, 0, Db3, 0}; // NL2
int melody_B_47b[] = {F2, 0, F3, 0}; // NL2
// M48
/* repeat M32 */
// M49
/* repeat M33 */
// M50
/* repeat M34 */


// M51
int melody_V1_51a[] = {0, Bb5, Ab5}; // NL5
int melody_V1_51b[] = {0, Bb5, Ab5, 0}; // NL2
int melody_V2_51a[] = {0, Ab4, 0, Ab4}; // NL2
int melody_V2_51b[] = {0, Ab4, 0, Ab4}; // NL2
int melody_B_51a[] = {D4, 0, Bb4, 0}; // NL2
int melody_B_51b[] = {D4, 0, Bb4, 0}; // NL2
// M52
/* repeat M51 */
// M53
int melody_V1_53a[] = {Ab5, Bb5, Ab5, F5}; // NL7
int melody_V1_53b[] = {F5, E5, F5}; // NL5
int melody_V2_53a[] = {0, C5, 0, C5}; // NL2
int melody_V2_53b[] = {0, C5, 0, C5}; // NL2
int melody_B_53a[] = {F4, 0, D5, 0}; // NL2
int melody_B_53b[] = {F4, 0, D5, 0}; // NL2
// M54
int melody_V1_54a[] = {F5}; // NL0
int melody_V1_54b[] = {0}; // NL0
/* repeat M53_V2 & B */
// M55
/* repeat M51 */


// M56
/* repeat M51 */
// M57
int melody_V1_57a[] = {Gs5, As5}; // NL1
int melody_V1_57b[] = {B5, As5, 0, G5}; // NL2
/* repeat M53_V2 & B */
// M58
/* repeat M54 */
// M59
/* repeat M51 */
// M60
/* repeat M52 */


// M61
/* repeat M53 */
// M62
/* repeat M54 */
// M63
/* repeat M55 */
// M64
/* repeat M56 */
// M65
/* repeat M57 */


// M66
/* repeat M58 */
// M67
int melody_V1_67a[] = {0, Bb4, Ab4}; // NL5
int melody_V1_67b[] = {0, Bb4, Ab4, 0}; // NL2
int melody_V2_67a[] = {0, Ab3, 0, Ab3}; // NL2
int melody_V2_67b[] = {0, Ab3, 0, Ab3}; // NL2
int melody_B_67a[] = {D2, 0, Bb3, 0}; // NL2
int melody_B_67b[] = {D2, 0, Bb3, 0}; // NL2
// M68
/* repeat M67 */
// M69
int melody_V1_69a[] = {Ab4, Bb4, Ab4, F4}; // NL7
int melody_V1_69b[] = {F4, E4, F4}; // NL5
int melody_V2_69a[] = {0, C3, 0, C3}; // NL2
int melody_V2_69b[] = {0, C3, 0, C3}; // NL2
int melody_B_69a[] = {F2, 0, Cs3, 0}; // NL2
int melody_B_69b[] = {F2, 0, Cs3, 0}; // NL2
// M70
int melody_V1_70a[] = {F4}; // NL0
int melody_V1_70b[] = {0}; // NL0
/* repeat M53_V2 & B */


// M71
/* repeat M67 */
// M72
/* repeat M67 */
// M73
int melody_V1_73a[] = {Ab4, Bb4}; // NL1
int melody_V1_73b[] = {B4, Bb4, 0, Ab4}; // NL2
/* repeat M69_V2 & B */
// M74
/* repeat M70 */
// M75
/* repeat M67 */


// M76
/* repeat M67 */
// M77
int melody_V1_77a[] = {C5, C5}; // NL1
int melody_V1_77b[] = {E5, C5, Bb4}; // NL6
int melody_V2_77a[] = {C6, C6}; // NL1
int melody_V2_77b[] = {E6, C6, Bb5}; // NL6
int melody_B_77a[] = {C4, C4}; // NL1
int melody_B_77b[] = {E4, C4, Bb3}; // NL6
// M78
int melody_V1_78a[] = {C6, C6, 0, C6, 0}; // NL8
int melody_V1_78b[] = {C6, 0, C6, 0, C6, 0, C6, 0}; // NL9
int melody_V2_78a[] = {0, 0, Db6, 0, Db6}; // NL8
int melody_V2_78b[] = {0, Db6, 0, Db6, 0, Db6, 0, Db6}; // NL9
int melody_B_78a[] = {C4, 0, E4, F4}; // NL10
int melody_B_78b[] = {G4}; // NL0
// M79
int melody_V1_79a[] = {C5, A4}; // NL1
int melody_V1_79b[] = {F4, C4}; // NL1
int melody_B_79a[] = {F2, 0}; // NL1
int melody_B_79b[] = {0}; // NL0
/* V2 empty */

// M80
int melody_V1_80a[] = {Ab4, F4, G4}; // NL5
int melody_V1_80b[] = {G4, 0}; // NL1
int melody_B_80a[] = {Bb2, 0}; // NL1
int melody_B_80b[] = {0}; // NL0
/* V2 empty */

// M81
int melody_V1_81a[] = {Ab4, F4, G4}; // NL5
int melody_V1_81b[] = {G4, 0}; // NL1
int melody_B_81a[] = {Db2, 0}; // NL1
int melody_B_81b[] = {0}; // NL0
/* V2 empty */

// M82
int melody_V2_82a[] = {Ab4, F4, G4}; // NL5
int melody_V2_82b[] = {G4, Db4, C4, Db4}; // NL2
int melody_B_82a[] = {C3, 0}; // NL1
int melody_B_82b[] = {C3, E2}; // NL1
/* V2 empty */

// M83
/* repeat M31_V1 */
int melody_B_83a[] = {F2, 0}; // NL1
int melody_B_83b[] = {0}; // NL0

// M84
/* repeat M32_V1 */
int melody_B_84a[] = {Bb2, 0}; // NL1
int melody_B_84b[] = {0}; // NL0

// M85
/* repeat M33_V1 */
int melody_B_85a[] = {Db3, 0}; // NL1
int melody_B_85b[] = {C3, 0}; // NL1

// M86
/* repeat M34_V1 */
int melody_B_86a[] = {C3, E2}; // NL1
int melody_B_86b[] = {F2}; // NL0
