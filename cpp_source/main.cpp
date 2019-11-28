#include <iostream>
#include <cstdio>
#include <algorithm>
#include<ctime>
using namespace std;


class Data
{
  private:
    const int NO_STRAIGHT_NOR_FLUSH = -2;
    const int AT_LEAST_STRAIGHT_FLUSH = -1;
    const int NO_MATCHES = 0;
    const int IS_ROYAL_FLUSH = 1;
    const int IS_STRAIGHT_FLUSH = 2;
    const int IS_4_OF_A_KIND = 3;
    const int IS_FULL_HOUSE = 4;
    const int IS_FLUSH = 5;
    const int IS_STRAIGHT = 6;
    const int IS_3_OF_A_KIND = 7;
    const int IS_2_PAIR = 8;
    const int IS_PAIR = 9;

    // const int ARRAY_RANK_SIZE = 14;

    int s1, s2, s3, s4, s5, s6, s7;
    int r1, r2, r3, r4, r5, r6, r7;
    int cardMatrix[5][15] = {0}; //Values (0 or 1) for suits are in slots 1-4, values for ranks are in 1-13. Use int instead of bool for better performance

    int aces, twos, threes, fours, fives, sixes, sevens, eights, nines, tens, jacks, queens, kings;
    int ranks[15];

    int handRanking;
    int highCard1 = 0, highCard2 = 0, highCard3 = 0, highCard4 = 0, highCard5 = 0; //0 when not applicable, or from 1-14 where 1 is aces low and 14 is aces high

    // Because of the 7-card limit, having a straight and/or a flush rules out the possibility of a 4K or FH.
    // Values are: 0 or 1

    // int containsStraight, containsFlush, numberOfRepeats, largestMatch, straightCombo;
    int containsStraight = 0, containsFlush = 0, numberOfRepeats = 0, largestMatch = 0, straightCombo = 0;

    // int containsStraight = 0; // If 1 / true, 4K and FH are impossible
    // int containsFlush = 0;    // If 1 / true, 4K and FH are impossible
    // int numberOfRepeats = 0;  // Quantity of ranks where you have duplicates (pairs, triples, or quads)
    // int largestMatch;         // 4 if you have a quad, 3 if you have a triple, 2 if you have a pair, and 1 if you have no duplicates of any rank
    // int straightCombo = 0;    // A counter for how many consecutive ranks you have

    //rename numberOfRepeats to nOfPairsTriplesAndQuads
    //rename largestMatch to mostMatchesOfSameRank
    //rename straightCombo to mostConsecutiveRanks

    int highCard1, highCard2, highCard3, highCard4, highCard5;

  public:
    Data(
        int suit1, int rank1,
        int suit2, int rank2,
        int suit3, int rank3,
        int suit4, int rank4,
        int suit5, int rank5,
        int suit6, int rank6,
        int suit7, int rank7)
    {
      s1 = suit1;
      r1 = rank1;
      s2 = suit2;
      r2 = rank2;
      s3 = suit3;
      r3 = rank3;
      s4 = suit4;
      r4 = rank4;
      s5 = suit5;
      r5 = rank5;
      s6 = suit6;
      r6 = rank6;
      s7 = suit7;
      r7 = rank7;
      }

    void setMatrixCounts()
    {
      //stores each of the 7 cards into a matrix where order doesn't matter
      int cards[14] = {s1, r1, s2, r2, s3, r3, s4, r4, s5, r5, s6, r6, s7, r7};

      for(int i = 0; i <= 12; i+=2)
        cardMatrix[i][i+1] = 1;

      // Set the values of the high aces
      for(int i = 1; i <= 4; i++)
        cardMatrix[i][14] = cardMatrix[i][1];
    }
    void setMatrixCounts_2()
    {
      //stores each of the 7 cards into a matrix where order doesn't matter
      int cards[14] = {s1, r1, s2, r2, s3, r3, s4, r4, s5, r5, s6, r6, s7, r7};

      cardMatrix[s1][r1] = cardMatrix[s2][r2] = cardMatrix[s3][r3] = cardMatrix[s4][r4] = cardMatrix[s5][r5] = cardMatrix[s6][r6] = cardMatrix[s7][r7] = 1;

      // Set the values of the high aces
      for (int i = 1; i <= 4; i++)
        cardMatrix[i][14] = cardMatrix[i][1];

    }
    void setRankCounts()
    {
      aces = cardMatrix[1][1] + cardMatrix[2][1] + cardMatrix[3][1] + cardMatrix[4][1];
      twos = cardMatrix[1][2] + cardMatrix[2][2] + cardMatrix[3][2] + cardMatrix[4][2];
      threes = cardMatrix[1][3] + cardMatrix[2][3] + cardMatrix[3][3] + cardMatrix[4][3];
      fours = cardMatrix[1][4] + cardMatrix[2][4] + cardMatrix[3][4] + cardMatrix[4][4];
      fives = cardMatrix[1][5] + cardMatrix[2][5] + cardMatrix[3][5] + cardMatrix[4][5];
      sixes = cardMatrix[1][6] + cardMatrix[2][6] + cardMatrix[3][6] + cardMatrix[4][6];
      sevens = cardMatrix[1][7] + cardMatrix[2][7] + cardMatrix[3][7] + cardMatrix[4][7];
      eights = cardMatrix[1][8] + cardMatrix[2][8] + cardMatrix[3][8] + cardMatrix[4][8];
      nines = cardMatrix[1][9] + cardMatrix[2][9] + cardMatrix[3][9] + cardMatrix[4][9];
      tens = cardMatrix[1][10] + cardMatrix[2][10] + cardMatrix[3][10] + cardMatrix[4][10];
      jacks = cardMatrix[1][11] + cardMatrix[2][11] + cardMatrix[3][11] + cardMatrix[4][11];
      queens = cardMatrix[1][12] + cardMatrix[2][12] + cardMatrix[3][12] + cardMatrix[4][12];
      kings = cardMatrix[1][13] + cardMatrix[2][13] + cardMatrix[3][13] + cardMatrix[4][13];

      // To make it easier to keep track of my array indices, I set index 0 to a value of 0 and don't intend on using it.
      // It's easier to trace my code if I can know at a glance that ranks[4] corresponds to a card rank of 4 etc.
      // Aces at the bottom and the top to make it easier to code in cases where I need to deal with one or the other.
      int newRanks[15] = {0, aces, twos, threes, fours, fives, sixes, sevens, eights, nines, tens, jacks, queens, kings, aces};
      for(int i = 0; i < 15; i++)
        ranks[i] = newRanks[i];
    }
    void quickCheckForFlush()
    {
      //DO 1ST BECAUSE IT'S SUPER FAST AND CAN RULE OUT MANY POSSIBLE HANDS WHEN IT FINDS A FLUSH
      // If 1 / true, everything above it is impossible
      containsFlush = (s1 == s5 || s2 == s6 || s3 == s7);
      //This hand is at least a flush, and possibly a straight/royal flush
      //This hand is definitely not a 4-of a kind or full house
    }
    void quickCheckForStraight()
    {
      // This check has the form: (fives > 0 || tens > 0) && ....
      // so that many definite (no 5s or 10s) none-straights will be ruled out immediately
      containsStraight = (fives > 0 || tens > 0)
        && (((aces > 0 && twos > 0 && threes > 0 && fours > 0 && fives > 0)
        || (twos > 0 && threes > 0 && fours > 0 && fives > 0 && sixes > 0)
        || (threes > 0 && fours > 0 && fives > 0 && sixes > 0 && sevens > 0)
        || (fours > 0 && fives > 0 && sixes > 0 && sevens > 0 && eights > 0)
        || (fives > 0 && sixes > 0 && sevens > 0 && eights > 0 && nines > 0)
        || (sixes > 0 && sevens > 0 && eights > 0 && nines > 0 && tens > 0)
        || (sevens > 0 && eights > 0 && nines > 0 && tens > 0 && jacks > 0)
        || (eights > 0 && nines > 0 && tens > 0 && jacks > 0 && queens > 0)
        || (nines > 0 && tens > 0 && jacks > 0 && queens > 0 && kings > 0)
        || (tens > 0 && jacks > 0 && queens > 0 && kings > 0 && aces > 0)));
    }
    void quickCheckForRepeats()
    {
      for (int i = 1; i <= 13; i++)
        numberOfRepeats += (ranks[i] >= 2);
    }
    void quickCheckForRepeats_2()
    {
      for (int i = 1; i <= 13; i++)
        if (ranks[i] >= 2)
          numberOfRepeats++;
    }

    void testStraightAndRepeatedRanks() // Not needed if I use quickCheckForStraight() & quickCheckForRepeats()
    {
      // The problem with this approach is that it will keep checking for straight even if it already found one, and it will keep looking for
      // repeats even after finding two. I can't add more checks to prevent unnecessary checks because that also adds to the overhead.
      // It's also hard to read since I'm trying to do two things at once.


      // used to determine if you got a 4K, FH, 3K, 2P, P, or no pair (used in conditionals at varying stages of this method)
      // Can also rule out all forms of F and S since they are mutually exlusive with 4K and FH
      

      // low aces to fours
      for (int i = 1; i <= 4; i++)
      {
        if (ranks[i] > 0)
        {
          straightCombo++;
          if (ranks[i] >= 2)
            numberOfRepeats++;
        }
        else
          straightCombo = 0;
      }

      // fives to kings
      for (int i = 5; i <= 13; i++)
      {
        if (ranks[i] > 0)
        {
          straightCombo++;
          if (straightCombo >= 5)
            containsStraight = 1;
          if (ranks[i] >= 2)
            numberOfRepeats++;
        }
        else
          straightCombo = 0;
      }

      // high aces
      if (aces > 0)
      {
        straightCombo++;
        if (straightCombo >= 5) 
          containsStraight = 1;
      }
    }
    void testStraightAndRepeatRanks_2()
    {
      // The problem with this approach is that I'm still using branches
      int cardFrequency[] = {aces, twos, threes, fours, fives, sixes, sevens, eights, nines, tens, jacks, queens, kings};

      for (int i = 0; i < 9; i++) // check first 9 cards
      {
        if (cardFrequency[i] != 0)
        {
          straightCombo++;
          if (straightCombo == 5)
          {
            containsStraight = 1;
            break;
          }
        }
        else
          straightCombo = 0;
      }
      if (containsStraight == 1 || straightCombo == 0) // either there is a straight, or there is no way to make a straight with the remaining cards
        for (int i = 9; i < 13; i++)                   // last 4 cards
        {
          if (cardFrequency[i] != 0)
          {
            straightCombo++;
            if (straightCombo == 5)
              containsStraight = 1;
          }
        }
    }
    void testStraightAndRepeatedRanks_3()
    {
      //1. This approach is ugly, but it allows me to minimize branching
      if ((aces > 0 && twos > 0 && threes > 0 && fours > 0 && fives > 0)
      || (twos > 0 && threes > 0 && fours > 0 && fives > 0 && sixes > 0)
      || (threes > 0 && fours > 0 && fives > 0 && sixes > 0 && sevens > 0)
      || (fours > 0 && fives > 0 && sixes > 0 && sevens > 0 && eights > 0)
      || (fives > 0 && sixes > 0 && sevens > 0 && eights > 0 && nines > 0)
      || (sixes > 0 && sevens > 0 && eights > 0 && nines > 0 && tens > 0)
      || (sevens > 0 && eights > 0 && nines > 0 && tens > 0 && jacks > 0)
      || (eights > 0 && nines > 0 && tens > 0 && jacks > 0 && queens > 0)
      || (nines > 0 && tens > 0 && jacks > 0 && queens > 0 && kings > 0))
      {
          containsStraight = 1;
      }

      numberOfRepeats = (aces >= 2) + (twos >= 2) + (threes >= 2)  + (fours >= 2)
      + (fives >= 2)  + (sixes >= 2)  + (sevens >= 2)
      + (eights >= 2)  + (nines >= 2)  + (tens >= 2)
      + (jacks >= 2)  + (queens >= 2)  + (kings >= 2);
    }
    void testStraightAndRepeatedRanks_4()
    {
      if (aces > 0)   { straightCombo++; if (aces >= 2)   {numberOfRepeats++;} } else {straightCombo = 0;}
      if (twos > 0)   { straightCombo++; if (twos >= 2)   {numberOfRepeats++;} } else {straightCombo = 0;}
      if (threes > 0) { straightCombo++; if (threes >= 2) {numberOfRepeats++;} } else {straightCombo = 0;}
      if (fours > 0)  { straightCombo++; if (fours >= 2)  {numberOfRepeats++;} } else {straightCombo = 0;}
      if (fives > 0)  { straightCombo++; if (straightCombo >= 5) {containsStraight = 1; } if (fives >= 2)  {numberOfRepeats++;} } else {straightCombo = 0;}
      if (sixes > 0)  { straightCombo++; if (straightCombo >= 5) {containsStraight = 1; } if (sixes >= 2)  {numberOfRepeats++;} } else {straightCombo = 0;}
      if (sevens > 0) { straightCombo++; if (straightCombo >= 5) {containsStraight = 1; } if (sevens >= 2) {numberOfRepeats++;} } else {straightCombo = 0;}
      if (eights > 0) { straightCombo++; if (straightCombo >= 5) {containsStraight = 1; } if (eights >= 2) {numberOfRepeats++;} } else {straightCombo = 0;}
      if (nines > 0)  { straightCombo++; if (straightCombo >= 5) {containsStraight = 1; } if (nines >= 2)  {numberOfRepeats++;} } else {straightCombo = 0;}
      if (tens > 0)   { straightCombo++; if (straightCombo >= 5) {containsStraight = 1; } if (tens >= 2)   {numberOfRepeats++;} } else {straightCombo = 0;}
      if (jacks > 0)  { straightCombo++; if (straightCombo >= 5) {containsStraight = 1; } if (jacks >= 2)  {numberOfRepeats++;} } else {straightCombo = 0;}
      if (queens > 0) { straightCombo++; if (straightCombo >= 5) {containsStraight = 1; } if (queens >= 2) {numberOfRepeats++;} } else {straightCombo = 0;}
      if (kings > 0)  { straightCombo++; if (straightCombo >= 5) {containsStraight = 1; } if (kings >= 2)  {numberOfRepeats++;} } else {straightCombo = 0;}
      if (aces > 0)   { straightCombo++; if (straightCombo >= 5) {containsStraight = 1; } }
    }


    int hasStraightButNoFlush(){
      return containsFlush == 0 && containsStraight == 1;
    }
    int hasFlushButNoStraight(){
      return containsFlush == 1 && containsStraight == 0;
    }
    int hasFlushAndStraight(){
      return containsFlush == 1 && containsStraight == 1;
    }
    int isStraightFlushOrBetter(){
      return (s1 == s5 && r5 - r1 == 4)
          || (s2 == s6 && r6 - r2 == 4)
          || (s3 == s7 && r7 - r3 == 4);
    }
    int isNotRoyalFlush(){
      return aces == 0
          || kings == 0
          || queens == 0
          || jacks == 0
          || tens == 0;
    }
    int checkForStraightAndFlush() //rename
    {
      //Quickly rule out anything that isn't a flush, straight, or straight/royal flush
      if (hasStraightButNoFlush())
        return IS_STRAIGHT;
      else if (hasFlushButNoStraight())
        return IS_FLUSH;
      else if (hasFlushAndStraight()) // Very quick check to rule out all straight/royal flushes
      {
        if (isStraightFlushOrBetter())
        {
          if (isNotRoyalFlush())
            return IS_STRAIGHT_FLUSH;

          return AT_LEAST_STRAIGHT_FLUSH;
        }

        return IS_FLUSH;
      }
      else
        return NO_STRAIGHT_NOR_FLUSH;
    }
    int checkForMatchedRanks()
    {
      int largestMatch = twos; // Start off with how many two's you have
      highCard1 = 2;

      for(int i = 3; i <= 14; i++)
        if (ranks[i] >= largestMatch)
        {
          largestMatch = ranks[i];
          highCard1 = i;
          break;
        }

      if (largestMatch == 4) //If it's a 4oaK, there is no way it could be anything higher
        return IS_4_OF_A_KIND;
      else if (largestMatch == 3 && numberOfRepeats >= 2) //Possible to have two triples, or two doubles with a triple
        return IS_FULL_HOUSE;
      else if (largestMatch == 3 && numberOfRepeats == 1)
        return IS_3_OF_A_KIND;
      else if (numberOfRepeats >= 2)
        return IS_2_PAIR;
      else if (numberOfRepeats == 1)
        return IS_PAIR;
      else
        // hand = 10;
        // goto handDecided;
        return NO_MATCHES;
    }
    int checkForMatchedRanks_2()
    {
      int largestMatch = twos; // Start off with how many two's you have
      highCard1 = 2;

      if (threes >= largestMatch)
      {
        largestMatch = threes;
        highCard1 = 3;
      }
      if (fours >= largestMatch)
      {
        largestMatch = fours;
        highCard1 = 4;
      }
      if (fives >= largestMatch)
      {
        largestMatch = fives;
        highCard1 = 5;
      }
      if (sixes >= largestMatch)
      {
        largestMatch = sixes;
        highCard1 = 6;
      }
      if (sevens >= largestMatch)
      {
        largestMatch = sevens;
        highCard1 = 7;
      }
      if (eights >= largestMatch)
      {
        largestMatch = eights;
        highCard1 = 8;
      }
      if (nines >= largestMatch)
      {
        largestMatch = nines;
        highCard1 = 9;
      }
      if (tens >= largestMatch)
      {
        largestMatch = tens;
        highCard1 = 10;
      }
      if (jacks >= largestMatch)
      {
        largestMatch = jacks;
        highCard1 = 11;
      }
      if (queens >= largestMatch)
      {
        largestMatch = queens;
        highCard1 = 12;
      }
      if (kings >= largestMatch)
      {
        largestMatch = kings;
        highCard1 = 13;
      }
      if (aces >= largestMatch)
      {
        largestMatch = aces;
        highCard1 = 14;
      }

      // if (largestMatch == 4){}
          //definitely a 4oaK
      // if (largestMatch == 3 && atLeast2Pair >= 2){} //possible to have two triples, or two doubles with a triple
          //definitely a full house
      // if (largestMatch == 3 && atLeast2Pair == 1){}
          // Probably a 3oaK, need to rule out flush and straight
      // if (atLeast2Pair == 3){}
          // The only type of 2 pair that can't be a flush or a straight
      // if (atLeast2Pair == 2){}
          // Probably a 2 pair, need to rule out flush and straight
      // if (atLeast2Pair == 1){}
          // Probably a  pair, need to rule out flush and straight

      if (largestMatch == 4) //If it's a 4oaK, there is no way it could be anything higher
        return IS_4_OF_A_KIND;
      else if (largestMatch == 3 && numberOfRepeats >= 2) //Possible to have two triples, or two doubles with a triple
        return IS_FULL_HOUSE;
      else if (largestMatch == 3 && numberOfRepeats == 1)
        return IS_3_OF_A_KIND;
      else if (numberOfRepeats >= 2)
        return IS_2_PAIR;
      else if (numberOfRepeats == 1)
        return IS_PAIR;
      else
        // hand = 10;
        // goto handDecided;
        return NO_MATCHES;
    }



    int checkIfRoyalFlush()
    {
      for (int s = 1; s <= 4; s++)
      {
        int consecutiveHighCards = 0;

        for (int r = 10; r <= 14; r++)
          consecutiveHighCards += cardMatrix[s][r];
        
        if (consecutiveHighCards == 5)
          return IS_ROYAL_FLUSH;
      }
    }
    int checkIfRoyalFlush_2()
    {
      if ((cardMatrix[1][1] + cardMatrix[1][10] + cardMatrix[1][11] + cardMatrix[1][12] + cardMatrix[1][13] == 5)
       || (cardMatrix[2][1] + cardMatrix[2][10] + cardMatrix[2][11] + cardMatrix[2][12] + cardMatrix[2][13] == 5)
       || (cardMatrix[3][1] + cardMatrix[3][10] + cardMatrix[3][11] + cardMatrix[3][12] + cardMatrix[3][13] == 5)
       || (cardMatrix[4][1] + cardMatrix[4][10] + cardMatrix[4][11] + cardMatrix[4][12] + cardMatrix[4][13] == 5))
      {
        return IS_ROYAL_FLUSH;
      }
    }
    void highCardForStraightFlush()
    {
      if (s1 == s5 && r5 - r1 == 4)
        highCard1 = r5;
      if (s2 == s6 && r6 - r2 == 4)
        highCard1 = r6;
      if (s3 == s7 && r7 - r3 == 4)
        highCard1 = r7;

      // return IS_STRAIGHT_FLUSH;
    }
    void highCardForStraightFlush_2()
    {
      for (int s = 1; s <= 4; s++)
        for (int r = 9; r >= 1; r--)
          if (cardMatrix[s][r] + cardMatrix[s][r+1] + cardMatrix[s][r+2] + cardMatrix[s][r+3] + cardMatrix[s][r+4] == 5)
          {
            highCard1 = r + 4;
            // return IS_STRAIGHT_FLUSH;
          }
    }
    int checkIf4OfAKind()
    {
      if (aces == 4 || twos == 4 || threes == 4
           || fours == 4 || fives == 4 || sixes == 4
           || sevens == 4 || eights == 4 || nines == 4
           || tens == 4 || jacks == 4 || queens == 4 || kings == 4)
      {
        return IS_4_OF_A_KIND;
      }        
    }
    void highCardFor4OfAKind()
    {
      for (int i = 14; i <= 2; i--)
        if (ranks[i] == 4)
        {
          highCard1 = i;
          break;
        }

      for (int i = 14; i <= 2; i--)
        if (ranks[i] > 0 && ranks[i] < 4)
        {
          highCard2 = i;
          break;
        }
    }
    void highCardFor4OfAKind_2()
    {
      if (aces == 4)
        highCard1 = 14;
      else if (kings == 4)
        highCard1 = 13;
      else if (queens == 4)
        highCard1 = 12;
      else if (jacks == 4)
        highCard1 = 11;
      else if (tens == 4)
        highCard1 = 10;
      else if (nines == 4)
        highCard1 = 9;
      else if (eights == 4)
        highCard1 = 8;
      else if (sevens == 4)
        highCard1 = 7;
      else if (sixes == 4)
        highCard1 = 6;
      else if (fives == 4)
        highCard1 = 5;
      else if (fours == 4)
        highCard1 = 4;
      else if (threes == 4)
        highCard1 = 3;
      else if (twos == 4)
        highCard1 = 2;

      if (aces > 0 && aces < 4)
        highCard2 = 14;
      else if (kings > 0 && kings < 4)
        highCard2 = 13;
      else if (queens > 0 && queens < 4)
        highCard2 = 12;
      else if (jacks > 0 && jacks < 4)
        highCard2 = 11;
      else if (tens > 0 && tens < 4)
        highCard2 = 10;
      else if (nines > 0 && nines < 4)
        highCard2 = 9;
      else if (eights > 0 && eights < 4)
        highCard2 = 8;
      else if (sevens > 0 && sevens < 4)
        highCard2 = 7;
      else if (sixes > 0 && sixes < 4)
        highCard2 = 6;
      else if (fives > 0 && fives < 4)
        highCard2 = 5;
      else if (fours > 0 && fours < 4)
        highCard2 = 4;
      else if (threes > 0 && threes < 4)
        highCard2 = 3;
      else if (twos > 0 && twos < 4)
        highCard2 = 2;
    }
    int checkIfFullHouse()
    {
      if ( (((aces >= 2) + (twos >= 2) + (threes >= 2)
          + (fours >= 2) + (fives >= 2) + (sixes >= 2)
          + (sevens >= 2) + (eights >= 2) + (nines >= 2)
          + (tens >= 2) + (jacks >= 2) + (queens >= 2) + (kings >= 2))  >= 2) //check if you have at least two pairs, one of these will also be a triple
          && (((aces == 3) + (twos == 3) + (threes == 3)
          + (fours == 3) + (fives == 3) + (sixes == 3)
          + (sevens == 3) + (eights == 3) + (nines == 3)
          + (tens == 3) + (jacks == 3) + (queens == 3) + (kings == 3))  >= 1)
         )
      {
        return IS_FULL_HOUSE;
      }
    }
    void highCardForFullHouse()
    {
      for (int i = 14; i <= 2; i--)
        if (ranks[i] == 3)
        {
          highCard1 = i;
          break;
        }

      for (int i = 14; i <= 2; i--)
        if (ranks[i] >= 2 && highCard1 != i)
        {
          highCard2 = i;
          break;
        }
    }
    void highCardForFullHouse_2()
    {
      if (aces == 3)
        highCard1 = 14;
      else if (kings == 3)
        highCard1 = 13;
      else if (queens == 3)
        highCard1 = 12;
      else if (jacks == 3)
        highCard1 = 11;
      else if (tens == 3)
        highCard1 = 10;
      else if (nines == 3)
        highCard1 = 9;
      else if (eights == 3)
        highCard1 = 8;
      else if (sevens == 3)
        highCard1 = 7;
      else if (sixes == 3)
        highCard1 = 6;
      else if (fives == 3)
        highCard1 = 5;
      else if (fours == 3)
        highCard1 = 4;
      else if (threes == 3)
        highCard1 = 3;
      else if (twos == 3)
        highCard1 = 2;

      if (aces >= 2 && highCard1 != 14)
        highCard2 = 14;
      // == 2 could miss out on a lower triple, but >= 2 alone could end up counting the higher triple from the previous == 3 branches.
      else if (kings >= 2 && highCard1 != 13)
        highCard2 = 13;
      else if (queens >= 2 && highCard1 != 12)
        highCard2 = 12;
      else if (jacks >= 2 && highCard1 != 11)
        highCard2 = 11;
      else if (tens >= 2 && highCard1 != 10)
        highCard2 = 10;
      else if (nines >= 2 && highCard1 != 9)
        highCard2 = 9;
      else if (eights >= 2 && highCard1 != 8)
        highCard2 = 8;
      else if (sevens >= 2 && highCard1 != 7)
        highCard2 = 7;
      else if (sixes >= 2 && highCard1 != 6)
        highCard2 = 6;
      else if (fives >= 2 && highCard1 != 5)
        highCard2 = 5;
      else if (fours >= 2 && highCard1 != 4)
        highCard2 = 4;
      else if (threes >= 2 && highCard1 != 3)
        highCard2 = 3;
      else if (twos >= 2 && highCard1 != 2)
        highCard2 = 2;
    }
    int checkIfFlush_2()
    {
      for (int a = 1; a <= 4; a++)
        if (cardMatrix[a][1] + cardMatrix[a][2] + cardMatrix[a][3] + cardMatrix[a][4] + cardMatrix[a][5] + cardMatrix[a][6]
             + cardMatrix[a][7] + cardMatrix[a][8] + cardMatrix[a][9] + cardMatrix[a][10] + cardMatrix[a][11] + cardMatrix[a][12] + cardMatrix[a][13] >= 5)
        {
          return IS_FLUSH;
        }
    }
    void highCardForFlush()
    {
      int relevantSuit;

      if (s3 == s7)
        relevantSuit = s3;
      else if (s2 == s6)
        relevantSuit = s2;
      else if (s1 == s5)
        relevantSuit = s1;
      //else
      //  return 0;

      for (int i = 14; i <= 6; i--)
        if (cardMatrix[relevantSuit][i] == 1)
        {
          highCard1 = i;
          break;
        }

      for (int i = 13; i <= 5; i--)
        if (cardMatrix[relevantSuit][i] == 1 && highCard1 > i)
        {
          highCard2 = i;
          break;
        }

      for (int i = 12; i <= 4; i--)
        if (cardMatrix[relevantSuit][i] == 1 && highCard2 > i)
        {
          highCard3 = i;
          break;
        }

      for (int i = 11; i <= 3; i--)
        if (cardMatrix[relevantSuit][i] == 1 && highCard3 > i)
        {
          highCard4 = i;
          break;
        }

      for (int i = 10; i <= 2; i--)
        if (cardMatrix[relevantSuit][i] == 1 && highCard4 > i)
        {
          highCard5 = i;
          break;
        }
    }
    void highCardForFlush_2()
    {
      int relevantSuit;

      // OLD WAY
      // //conditions are all if, just in case there are more than 5 cards with matching suits, but the card on the right will have the highest rank
      // if (s1 == s5)
      //   a = s1;
      // if (s2 == s6)
      //   a = s2;
      // if (s3 == s7)
      //   a = s3;

      if (s3 == s7)
        relevantSuit = s3;
      else if (s2 == s6)
        relevantSuit = s2;
      else if (s1 == s5)
        relevantSuit = s1;      

      /*
      New: Test this out because it's not obvious if this works well

      Count how many cards share this suit (5 to 7) -> if (s1 == s6){} if (s1 == s7){} if (s2 == s7){}
      Create an int array of this length with default values of 0
      Check if any relevant (relevant = matches s# above) r# have an ace
          If there are no aces, fill array with relevant r1, r2, r3, r4 ,r5, r6, r7, and in that order (length and starting location can both vary)
          If there is an ace, fill array with relevant r2, r3, r4 ,r5, r6, r7, r1, and add 13 to r1
      highCard1 = int hcList[matchingSuits];
      highCard2 = int hcList[matchingSuits - 1];
      highCard3 = int hcList[matchingSuits - 2];
      highCard4 = int hcList[matchingSuits - 3];
      highCard5 = int hcList[matchingSuits - 4];
      */

      if (cardMatrix[relevantSuit][1] == 1)
        highCard1 = 14;
      else if (cardMatrix[relevantSuit][13] == 1)
        highCard1 = 13;
      else if (cardMatrix[relevantSuit][12] == 1)
        highCard1 = 12;
      else if (cardMatrix[relevantSuit][11] == 1)
        highCard1 = 11;
      else if (cardMatrix[relevantSuit][10] == 1)
        highCard1 = 10;
      else if (cardMatrix[relevantSuit][9] == 1)
        highCard1 = 9;
      else if (cardMatrix[relevantSuit][8] == 1)
        highCard1 = 8;
      else if (cardMatrix[relevantSuit][7] == 1)
        highCard1 = 7;
      else if (cardMatrix[relevantSuit][6] == 1)
        highCard1 = 6;

      if (cardMatrix[relevantSuit][13] == 1 && highCard1 > 13)
        highCard2 = 13;
      else if (cardMatrix[relevantSuit][12] == 1 && highCard1 > 12)
        highCard2 = 12;
      else if (cardMatrix[relevantSuit][11] == 1 && highCard1 > 11)
        highCard2 = 11;
      else if (cardMatrix[relevantSuit][10] == 1 && highCard1 > 10)
        highCard2 = 10;
      else if (cardMatrix[relevantSuit][9] == 1 && highCard1 > 9)
        highCard2 = 9;
      else if (cardMatrix[relevantSuit][8] == 1 && highCard1 > 8)
        highCard2 = 8;
      else if (cardMatrix[relevantSuit][7] == 1 && highCard1 > 7)
        highCard2 = 7;
      else if (cardMatrix[relevantSuit][6] == 1 && highCard1 > 6)
        highCard2 = 6;
      else if (cardMatrix[relevantSuit][5] == 1)
        highCard2 = 5;

      if (cardMatrix[relevantSuit][12] == 1 && highCard2 > 12)
        highCard3 = 12;
      else if (cardMatrix[relevantSuit][11] == 1 && highCard2 > 11)
        highCard3 = 11;
      else if (cardMatrix[relevantSuit][10] == 1 && highCard2 > 10)
        highCard3 = 10;
      else if (cardMatrix[relevantSuit][9] == 1 && highCard2 > 9)
        highCard3 = 9;
      else if (cardMatrix[relevantSuit][8] == 1 && highCard2 > 8)
        highCard3 = 8;
      else if (cardMatrix[relevantSuit][7] == 1 && highCard2 > 7)
        highCard3 = 7;
      else if (cardMatrix[relevantSuit][6] == 1 && highCard2 > 6)
        highCard3 = 6;
      else if (cardMatrix[relevantSuit][5] == 1 && highCard2 > 5)
        highCard3 = 5;
      else if (cardMatrix[relevantSuit][4] == 1)
        highCard3 = 4;

      if (cardMatrix[relevantSuit][11] == 1 && highCard3 > 11)
        highCard4 = 11;
      else if (cardMatrix[relevantSuit][10] == 1 && highCard3 > 10)
        highCard4 = 10;
      else if (cardMatrix[relevantSuit][9] == 1 && highCard3 > 9)
        highCard4 = 9;
      else if (cardMatrix[relevantSuit][8] == 1 && highCard3 > 8)
        highCard4 = 8;
      else if (cardMatrix[relevantSuit][7] == 1 && highCard3 > 7)
        highCard4 = 7;
      else if (cardMatrix[relevantSuit][6] == 1 && highCard3 > 6)
        highCard4 = 6;
      else if (cardMatrix[relevantSuit][5] == 1 && highCard3 > 5)
        highCard4 = 5;
      else if (cardMatrix[relevantSuit][4] == 1 && highCard3 > 4)
        highCard4 = 4;
      else if (cardMatrix[relevantSuit][3] == 1)
        highCard4 = 3;

      if (cardMatrix[relevantSuit][10] == 1 && highCard4 > 10)
        highCard5 = 10;
      else if (cardMatrix[relevantSuit][9] == 1 && highCard4 > 9)
        highCard5 = 9;
      else if (cardMatrix[relevantSuit][8] == 1 && highCard4 > 8)
        highCard5 = 8;
      else if (cardMatrix[relevantSuit][7] == 1 && highCard4 > 7)
        highCard5 = 7;
      else if (cardMatrix[relevantSuit][6] == 1 && highCard4 > 6)
        highCard5 = 6;
      else if (cardMatrix[relevantSuit][5] == 1 && highCard4 > 5)
        highCard5 = 5;
      else if (cardMatrix[relevantSuit][4] == 1 && highCard4 > 4)
        highCard5 = 4;
      else if (cardMatrix[relevantSuit][3] == 1 && highCard4 > 3)
        highCard5 = 3;
      else if (cardMatrix[relevantSuit][2] == 1)
        highCard5 = 2;
    }
    void highCardForStraight()
    {
      for (int i = 14; i >= 5; i--)
        if ((ranks[i] > 0) && (ranks[i - 1] > 0) && (ranks[i - 2] > 0) && (ranks[i - 3] > 0) && (ranks[i - 4] > 0))
          highCard1 = i;
    }
    void highCardForStraightt_2()
    {
      if (tens > 0 && jacks > 0 && queens > 0 && kings > 0 && aces > 0)
        highCard1 = 14;
      else if (nines > 0 && tens > 0 && jacks > 0 && queens > 0 && kings > 0)
        highCard1 = 13;
      else if (eights > 0 && nines > 0 && tens > 0 && jacks > 0 && queens > 0)
        highCard1 = 12;
      else if (sevens > 0 && eights > 0 && nines > 0 && tens > 0 && jacks > 0)
        highCard1 = 11;
      else if (sixes > 0 && sevens > 0 && eights > 0 && nines > 0 && tens > 0)
        highCard1 = 10;
      else if (fives > 0 && sixes > 0 && sevens > 0 && eights > 0 && nines > 0)
        highCard1 = 9;
      else if (fours > 0 && fives > 0 && sixes > 0 && sevens > 0 && eights > 0)
        highCard1 = 8;
      else if (threes > 0 && fours > 0 && fives > 0 && sixes > 0 && sevens > 0)
        highCard1 = 7;
      else if (twos > 0 && threes > 0 && fours > 0 && fives > 0 && sixes > 0)
        highCard1 = 6;
      else if (aces > 0 && twos > 0 && threes > 0 && fours > 0 && fives > 0)
        highCard1 = 5;
    }
    int checkIf3OfAKind()
    {
      if (aces == 3 || twos == 3 || threes == 3
       || fours == 3 || fives == 3 || sixes == 3
       || sevens == 3 || eights == 3 || nines == 3
       || tens == 3 || jacks == 3 || queens == 3 || kings == 3)
      {
        return IS_3_OF_A_KIND;
      }
    }
    void highCardFor3OfAKind()
    {
      for (int i = 14; i <= 2; i--)
        if (ranks[i] == 3)
          highCard1 = i;

      for (int i = 14; i <= 5; i--)
        if (ranks[i] == 1)
          highCard2 = i;

      for (int i = 13; i <= 4; i--)
        if (ranks[i] == 1 && highCard2 > i)
          highCard3 = i;
    }
    void highCardFor3OfAKind_2()
    {
      if (aces == 3)
        highCard1 = 14;
      else if (kings == 3)
        highCard1 = 13;
      else if (queens == 3)
        highCard1 = 12;
      else if (jacks == 3)
        highCard1 = 11;
      else if (tens == 3)
        highCard1 = 10;
      else if (nines == 3)
        highCard1 = 9;
      else if (eights == 3)
        highCard1 = 8;
      else if (sevens == 3)
        highCard1 = 7;
      else if (sixes == 3)
        highCard1 = 6;
      else if (fives == 3)
        highCard1 = 5;
      else if (fours == 3)
        highCard1 = 4;
      else if (threes == 3)
        highCard1 = 3;
      else if (twos == 3)
        highCard1 = 2;

      if (aces == 1)
        highCard2 = 14;
      // aces > 0 && aces < 3 also works but its more expensive and a bit more confusing to look at since aces == 2 means that this would be a Full House
      else if (kings == 1)
        highCard2 = 13;
      //It is possible for the two lesser cards to be higher than the triple, but they can't be equal in rank to each other
      else if (queens == 1)
        highCard2 = 12;
      else if (jacks == 1)
        highCard2 = 11;
      else if (tens == 1)
        highCard2 = 10;
      else if (nines == 1)
        highCard2 = 9;
      else if (eights == 1)
        highCard2 = 8;
      else if (sevens == 1)
        highCard2 = 7;
      else if (sixes == 1)
        highCard2 = 6;
      else if (fives == 1)
        highCard2 = 5;
      // Since the lowest possible remainders are 2, 3, 4, and 5, the lowest possible two kickers are 4|5
      //else if (fours == 1) { highCard2 = 4;    
      //else if (threes == 1) { highCard2 = 3;
      if (kings == 1 && highCard2 > 13)
        highCard3 = 13;
      else if (queens == 1 && highCard2 > 12)
        highCard3 = 12;
      else if (jacks == 1 && highCard2 > 11)
        highCard3 = 11;
      else if (tens == 1 && highCard2 > 10)
        highCard3 = 10;
      else if (nines == 1 && highCard2 > 9)
        highCard3 = 9;
      else if (eights == 1 && highCard2 > 8)
        highCard3 = 8;
      else if (sevens == 1 && highCard2 > 7)
        highCard3 = 7;
      else if (sixes == 1 && highCard2 > 6)
        highCard3 = 6;
      else if (fives == 1 && highCard2 > 5)
        highCard3 = 5;
      else if (fours == 1 && highCard2 > 4)
        highCard3 = 4;
      //else if (threes == 1 && highCard2 > 3) { highCard3 = 3;
      //else if (twos == 1) { highCard3 = 2; }
    }
    int checkIf2Pairs()
    {
      if (((aces == 2) + (twos == 2) + (threes == 2)
        + (fours == 2) + (fives == 2) + (sixes == 2)
        + (sevens == 2) + (eights == 2) + (nines == 2)
        + (tens == 2) + (jacks == 2) + (queens == 2) + (kings == 2))  >= 2)
      {
        return IS_2_PAIR;
      }      
    }
    void highCardFor2Pairs()
    {
      for (int i = 14; i <= 3; i--)
        if (ranks[i] == 2)
          highCard1 = i;

      for (int i = 13; i <= 2; i--)
        if (ranks[i] == 2 && highCard1 > i)
          highCard2 = i;

      for (int i = 14; i <= 3; i--)
        if (ranks[i] >= 1 && highCard1 != i && highCard2 != i)
          highCard3 = i;
    }
    int highCardFor2Pairs_2()
    {
      if (aces == 2)
        highCard1 = 14;
      else if (kings == 2)
        highCard1 = 13;
      else if (queens == 2)
        highCard1 = 12;
      else if (jacks == 2)
        highCard1 = 11;
      else if (tens == 2)
        highCard1 = 10;
      else if (nines == 2)
        highCard1 = 9;
      else if (eights == 2)
        highCard1 = 8;
      else if (sevens == 2)
        highCard1 = 7;
      else if (sixes == 2)
        highCard1 = 6;
      else if (fives == 2)
        highCard1 = 5;
      else if (fours == 2)
        highCard1 = 4;
      else if (threes == 2)
        highCard1 = 3;

      if (kings == 2 && highCard1 > 13)
        highCard2 = 13;
      else if (queens == 2 && highCard1 > 12)
        highCard2 = 12;
      else if (jacks == 2 && highCard1 > 11)
        highCard2 = 11;
      else if (tens == 2 && highCard1 > 10)
        highCard2 = 10;
      else if (nines == 2 && highCard1 > 9)
        highCard2 = 9;
      else if (eights == 2 && highCard1 > 8)
        highCard2 = 8;
      else if (sevens == 2 && highCard1 > 7)
        highCard2 = 7;
      else if (sixes == 2 && highCard1 > 6)
        highCard2 = 6;
      else if (fives == 2 && highCard1 > 5)
        highCard2 = 5;
      else if (fours == 2 && highCard1 > 4)
        highCard2 = 4;
      else if (threes == 2 && highCard1 > 3)
        highCard2 = 3;
      else if (twos == 2)
        highCard2 = 2;

      if (aces >= 1 && highCard1 != 14 && highCard2 != 14)
        highCard3 = 14;
      else if (kings >= 1 && highCard1 != 13 && highCard2 != 13)
        highCard3 = 13;
      else if (queens >= 1 && highCard1 != 12 && highCard2 != 12)
        highCard3 = 12;
      else if (jacks >= 1 && highCard1 != 11 && highCard2 != 11)
        highCard3 = 11;
      else if (tens >= 1 && highCard1 != 10 && highCard2 != 10)
        highCard3 = 10;
      else if (nines >= 1 && highCard1 != 9 && highCard2 != 9)
        highCard3 = 9;
      else if (eights >= 1 && highCard1 != 8 && highCard2 != 8)
        highCard3 = 8;
      else if (sevens >= 1 && highCard1 != 7 && highCard2 != 7)
        highCard3 = 7;
      else if (sixes >= 1 && highCard1 != 6 && highCard2 != 6)
        highCard3 = 6;
      else if (fives >= 1 && highCard1 != 5 && highCard2 != 5)
        highCard3 = 5;
      else if (fours >= 1 && highCard1 != 4 && highCard2 != 4)
        highCard3 = 4;
      // Since the lowest possible remainders are 2, 3, and 4, the lowest possible kicker is 4 - If the 3 remainers are unique
      else if (threes >= 1 && highCard1 != 3 && highCard2 != 3)
        highCard3 = 3;
      // If you have a 3rd pair, then 3 is the lowest possible kicker ( lone 3 and 2P 2, or lone 2 and 2P 3)
      //else if (twos == 1) { highCard3 = 2; }
    }
    int checkIfPair()
    {
      if (aces == 2 || twos == 2 || threes == 2
       || fours == 2 || fives == 2 || sixes == 2
       || sevens == 2 || eights == 2 || nines == 2
       || tens == 2 || jacks == 2 || queens == 2 || kings == 2)
      {
        return IS_PAIR;
      }
    }
    void highCardForPair()
    {
      for (int i = 14; i <= 2; i--)
        if (ranks[i] == 2)
          highCard1 = i;

      for (int i = 14; i <= 7; i--)
        if (ranks[i] == 1)
          highCard2 = i;

      for (int i = 13; i <= 5; i--)
        if (ranks[i] == 1 && highCard2 > i)
          highCard3 = i;

      for (int i = 12; i <= 4; i--)
        if (ranks[i] == 1 && highCard3 > i)
          highCard4 = i;
    }
    int highCardForPair_2()
    {
      if (aces == 2)
        highCard1 = 14;
      else if (kings == 2)
        highCard1 = 13;
      else if (queens == 2)
        highCard1 = 12;
      else if (jacks == 2)
        highCard1 = 11;
      else if (tens == 2)
        highCard1 = 10;
      else if (nines == 2)
        highCard1 = 9;
      else if (eights == 2)
        highCard1 = 8;
      else if (sevens == 2)
        highCard1 = 7;
      else if (sixes == 2)
        highCard1 = 6;
      else if (fives == 2)
        highCard1 = 5;
      else if (fours == 2)
        highCard1 = 4;
      else if (threes == 2)
        highCard1 = 3;
      else if (twos == 2)
        highCard1 = 2;

      if (aces == 1)
        highCard2 = 14;
      else if (kings == 1)
        highCard2 = 13;
      else if (queens == 1)
        highCard2 = 12;
      else if (jacks == 1)
        highCard2 = 11;
      else if (tens == 1)
        highCard2 = 10;
      else if (nines == 1)
        highCard2 = 9;
      else if (eights == 1)
        highCard2 = 8;
      else if (sevens == 1)
        highCard2 = 7;
      // Since the lowest possible remainders are 2, 3, 4, 5, and 6, the lowest "possible" three kickers are 4|5|6
      //else if (sixes == 1) highCard2 = 6; // 6 can't be the 1st kicker or else you are GUARANTEED a 2-3-4-5-6 straight
      //else if (fives == 1) highCard2 = 5;
      //else if (fours == 1) highCard2 = 4;

      if (kings == 1 && highCard2 > 13)
        highCard3 = 13;
      else if (queens == 1 && highCard2 > 12)
        highCard3 = 12;
      else if (jacks == 1 && highCard2 > 11)
        highCard3 = 11;
      else if (tens == 1 && highCard2 > 10)
        highCard3 = 10;
      else if (nines == 1 && highCard2 > 9)
        highCard3 = 9;
      else if (eights == 1 && highCard2 > 8)
        highCard3 = 8;
      else if (sevens == 1 && highCard2 > 7)
        highCard3 = 7;
      else if (sixes == 1 && highCard2 > 6)
        highCard3 = 6;
      else if (fives == 1 && highCard2 > 5)
        highCard3 = 5;
      //else if (fours == 1 && highCard2 > 4) highCard3 = 4;
      //else if (threes == 1) highCard3 = 3;

      if (queens == 1 && highCard3 > 12)
        highCard4 = 12;
      else if (jacks == 1 && highCard3 > 11)
        highCard4 = 11;
      else if (tens == 1 && highCard3 > 10)
        highCard4 = 10;
      else if (nines == 1 && highCard3 > 9)
        highCard4 = 9;
      else if (eights == 1 && highCard3 > 8)
        highCard4 = 8;
      else if (sevens == 1 && highCard3 > 7)
        highCard4 = 7;
      else if (sixes == 1 && highCard3 > 6)
        highCard4 = 6;
      else if (fives == 1 && highCard3 > 5)
        highCard4 = 5;
      else if (fours == 1 && highCard3 > 4)
        highCard4 = 4;
      //else if (threes == 1 && highCard3 > 3) highCard4 = 3;
      //else if (twos == 1) highCard4 = 2;
    }

    /*int checkIfHighCard()
    {
      //HIGH CARD - Check 5 high cards
      {
            if (aces == 1) { highCard1 = 14;}
            else if (kings == 1) { highCard1 = 13; }
            else if (queens == 1) { highCard1 = 12; }
            else if (jacks == 1) { highCard1 = 11; }
            else if (tens == 1) { highCard1 = 10; }
            else if (nines == 1) { highCard1 = 9; }
            else if (eights == 1) { highCard1 = 8; }
            else if (sevens == 1) { highCard1 = 7; }
            else if (sixes == 1) { highCard1 = 6; }

            if (kings == 1 && highCard1 > 13) { highCard2 = 13; }
            else if (queens == 1 && highCard1 > 12) { highCard2 = 12; }
            else if (jacks == 1 && highCard1 > 11) { highCard2 = 11; }
            else if (tens == 1 && highCard1 > 10) { highCard2 = 10; }
            else if (nines == 1 && highCard1 > 9) { highCard2 = 9; }
            else if (eights == 1 && highCard1 > 8) { highCard2 = 8; }
            else if (sevens == 1 && highCard1 > 7) { highCard2 = 7; }
            else if (sixes == 1 && highCard1 > 6) { highCard2 = 6; }
            else if (fives == 1) { highCard2 = 5; }

            if (queens == 1 && highCard2 > 12) { highCard3 = 12; }
            else if (jacks == 1 && highCard2 > 11) { highCard3 = 11; }
            else if (tens == 1 && highCard2 > 10) { highCard3 = 10; }
            else if (nines == 1 && highCard2 > 9) { highCard3 = 9; }
            else if (eights == 1 && highCard2 > 8) { highCard3 = 8; }
            else if (sevens == 1 && highCard2 > 7) { highCard3 = 7; }
            else if (sixes == 1 && highCard2 > 6) { highCard3 = 6; }
            else if (fives == 1 && highCard2 > 5) { highCard3 = 5; }
            else if (fours == 1) { highCard3 = 4; }

            if (jacks == 1 && highCard3 > 11) { highCard4 = 11; }
            else if (tens == 1 && highCard3 > 10) { highCard4 = 10; }
            else if (nines == 1 && highCard3 > 9) { highCard4 = 9; }
            else if (eights == 1 && highCard3 > 8) { highCard4 = 8; }
            else if (sevens == 1 && highCard3 > 7) { highCard4 = 7; }
            else if (sixes == 1 && highCard3 > 6) { highCard4 = 6; }
            else if (fives == 1 && highCard3 > 5) { highCard4 = 5; }
            else if (fours == 1 && highCard3 > 4) { highCard4 = 4; }
            else if (threes == 1) { highCard4 = 3; }

            if (tens == 1 && highCard4 > 10) { highCard5 = 10; }
            else if (nines == 1 && highCard4 > 9) { highCard5 = 9; }
            else if (eights == 1 && highCard4 > 8) { highCard5 = 8; }
            else if (sevens == 1 && highCard4 > 7) { highCard5 = 7; }
            else if (sixes == 1 && highCard4 > 6) { highCard5 = 6; }
            else if (fives == 1 && highCard4 > 5) { highCard5 = 5; }
            else if (fours == 1 && highCard4 > 4) { highCard5 = 4; }
            else if (threes == 1 && highCard4 > 3) { highCard5 = 3; }
            else if (twos == 1) { highCard5 = 2; }
        }
    }*/



    int handRankingRoyalFlush() // [1, 0 to 0]
    {
      return 0;
    }
    int handRankingStraightFlush() // [9, 1 to 9]
    {
      return handRanking = 14 - highCard1;
    }
    int handRanking4OfAKind() // [156, 10 to 165]
    {
      int counter;
      switch(highCard1)
      {
        case 14:
          counter = 9;
          break;
        case 13:
          counter = 21;
          break;
        case 12:
          counter = 33;
          break;
        case 11:
          counter = 45;
          break;
        case 10:
          counter = 57;
          break;
        case 9:
          counter = 69;
          break;
        case 8:
          counter = 81;
          break;
        case 7:
          counter = 93;
          break;
        case 6:
          counter = 105;
          break;
        case 5:
          counter = 117;
          break;
        case 4:
          counter = 129;
          break;
        case 3:
          counter = 141;
          break;
        default:
          counter = 153;  
      }

      if (highCard1 < highCard2)
        counter += (15 - highCard2);
      else
        counter += (14 - highCard2);

      return counter;
    }
    int handRanking4OfAKind_2() // [156, 10 to 165]
    {
      int counter;
      if (highCard1 == 14)
        counter = 9;
      else if (highCard1 == 13)
        counter = 21;
      else if (highCard1 == 12)
        counter = 33;
      else if (highCard1 == 11)
        counter = 45;
      else if (highCard1 == 10)
        counter = 57;
      else if (highCard1 == 9)
        counter = 69;
      else if (highCard1 == 8)
        counter = 81;
      else if (highCard1 == 7)
        counter = 93;
      else if (highCard1 == 6)
        counter = 105;
      else if (highCard1 == 5)
        counter = 117;
      else if (highCard1 == 4)
        counter = 129;
      else if (highCard1 == 3)
        counter = 141;
      else
        counter = 153;

      if (highCard1 < highCard2)
        counter += (15 - highCard2);
      else
        counter += (14 - highCard2);

      /*for (int first = highCard1; first >= 2; first--)
            for(int second = 14; second >= 2; second--)
                if (first != second)
                {
                    counter++;

                    if (first == highCard1 && second == highCard2)
                    {
                        handRanking = counter;
                        goto handRankingFound;
                    }
                }*/
      return counter;
    }
    int handRankingFullHouse() // [156, 166 to 321]
    {
      int counter;
      switch(highCard1)
      {
        case 14:
          counter = 165;
          break;
        case 13:
          counter = 177;
          break;
        case 12:
          counter = 189;
          break;
        case 11:
          counter = 201;
          break;
        case 10:
          counter = 213;
          break;
        case 9:
          counter = 225;
          break;
        case 8:
          counter = 237;
          break;
        case 7:
          counter = 249;
          break;
        case 6:
          counter = 261;
          break;
        case 5:
          counter = 273;
          break;
        case 4:
          counter = 285;
          break;
        case 3:
          counter = 297;
          break;
        default:
          counter = 309;  
      }

      if (highCard1 < highCard2)
        counter += (15 - highCard2);
      else
        counter += (14 - highCard2);

      return counter;
    }
    int handRankingFullHouse_2() // [156, 166 to 321]
    {
      int counter;
      if (highCard1 == 14)
        counter = 165;
      else if (highCard1 == 13)
        counter = 177;
      else if (highCard1 == 12)
        counter = 189;
      else if (highCard1 == 11)
        counter = 201;
      else if (highCard1 == 10)
        counter = 213;
      else if (highCard1 == 9)
        counter = 225;
      else if (highCard1 == 8)
        counter = 237;
      else if (highCard1 == 7)
        counter = 249;
      else if (highCard1 == 6)
        counter = 261;
      else if (highCard1 == 5)
        counter = 273;
      else if (highCard1 == 4)
        counter = 285;
      else if (highCard1 == 3)
        counter = 297;
      else
        counter = 309;

      if (highCard1 < highCard2)
        counter += (15 - highCard2);
      else
        counter += (14 - highCard2);

      return counter;
    }
    int handRankingFlush() // [1277, 322 to 1598]
    {
      int counter;
      switch(highCard1)
      {
        case 14:
          counter = 321;
          break;
        case 13:
          counter = 814;
          break;
        case 12:
          counter = 1143;
          break;
        case 11:
          counter = 1352;
          break;
        case 10:
          counter = 1477;
          break;
        case 9:
          counter = 1546;
          break;
        case 8:
          counter = 1580;
          break;
        default:
          counter = 1594;  
      }

      for (int first = highCard1; first >= 7; first--)
        for (int second = first - 1; second >= 5; second--)
          for (int third = second - 1; third >= 4; third--)
            for (int fourth = third - 1; fourth >= 3; fourth--)
              for (int fifth = fourth - 1; fifth >= 2; fifth--)                   //It's possible to have a Straight and a Flush in a 7-card combo that is not also a Straight Flush
                if (!((first == 14 && fifth == 10) || (first == 13 && fifth == 9) // avoid having any kind of straight
                      || (first == 12 && fifth == 8) || (first == 11 && fifth == 7) || (first == 10 && fifth == 6) || (first == 9 && fifth == 5) || (first == 8 && fifth == 4) || (first == 7 && fifth == 3) || (first == 6 && fifth == 2) || (first == 14 && second == 5)))
                {
                  counter++;

                  if (first == highCard1 && second == highCard2 && third == highCard3 && fourth == highCard4 && fifth == highCard5)
                  {
                    handRanking = counter;
                    goto handRankingFound;
                  }
                }
    }
    int handRankingFlush_2() // [1277, 322 to 1598]
    {
      int counter;
      if (highCard1 == 14)
        counter = 321;
      else if (highCard1 == 13)
        counter = 814;
      else if (highCard1 == 12)
        counter = 1143;
      else if (highCard1 == 11)
        counter = 1352;
      else if (highCard1 == 10)
        counter = 1477;
      else if (highCard1 == 9)
        counter = 1546;
      else if (highCard1 == 8)
        counter = 1580;
      else
        counter = 1594;

      for (int first = highCard1; first >= 7; first--)
        for (int second = first - 1; second >= 5; second--)
          for (int third = second - 1; third >= 4; third--)
            for (int fourth = third - 1; fourth >= 3; fourth--)
              for (int fifth = fourth - 1; fifth >= 2; fifth--) // It's possible to have a Straight and a Flush in a 7-card combo that is not also a Straight Flush
                if (!((first == 14 && fifth == 10) || (first == 13 && fifth == 9) // avoid having any kind of straight
                      || (first == 12 && fifth == 8) || (first == 11 && fifth == 7)
                      || (first == 10 && fifth == 6) || (first == 9 && fifth == 5)
                      || (first == 8 && fifth == 4) || (first == 7 && fifth == 3)
                      || (first == 6 && fifth == 2) || (first == 14 && second == 5)))
                {
                  counter++;

                  if (first == highCard1 && second == highCard2 && third == highCard3 && fourth == highCard4 && fifth == highCard5)
                  {
                    handRanking = counter;
                    goto handRankingFound;
                  }
                }
    }
    int handRankingStraight() // [10, 1599 to 1608]
    {
      return 1613 - highCard1;
    }
    int handRanking3OfAKind() // [575, 1609 to 2183]
    {
      int counter;
      switch(highCard1)
      {
        case 14:
          counter = 1608;
          break;
        case 13:
          counter = 1652;
          break;
        case 12:
          counter = 1697;
          break;
        case 11:
          counter = 1742;
          break;
        case 10:
          counter = 1787;
          break;
        case 9:
          counter = 1832;
          break;
        case 8:
          counter = 1877;
          break;
        case 7:
          counter = 1922;
          break;
        case 6:
          counter = 1967;
          break;
        case 5:
          counter = 2011;
          break;
        case 4:
          counter = 2054;
          break;
        case 3:
          counter = 2097;
          break;
        default:
          counter = 2140;  
      }

      for (int first = highCard1; first >= 2; first--)
        for (int second = 14; second >= 5; second--)
          if (first != second)
            for (int third = second - 1; third >= 4; third--)
              if ((first != third) && (!(first <= 4 && second == 6 && third == 5)                                                                                                                  //avoid having a 2-3-4-5-6 straight
                                      && !(first == 5 && second == 6 && third == 4) && !(first == 6 && second == 5 && third == 4) && !(first == 14 && second == 5 && third == 4)                  //avoid having a A-2-3-4-5 straight
                                      && !(first == 5 && second == 14 && third == 4) && !((first == 3 || first == 4) && second == 14 && third == 5) && !(first == 3 && second >= 5 && third == 4) //avoid impossible case
                                      && !(first == 2 && second == 14 && third == 5) && !(first == 2 && second >= 5 && third == 4)))
              {
                counter++;

                if (first == highCard1 && second == highCard2 && third == highCard3)
                {
                  handRanking = counter;
                  goto handRankingFound;
                }
              }
    }
    int handRanking3OfAKind_2() // [575, 1609 to 2183]
    {
      int counter;
      if (highCard1 == 14)
        counter = 1608;
      else if (highCard1 == 13)
        counter = 1652;
      else if (highCard1 == 12)
        counter = 1697;
      else if (highCard1 == 11)
        counter = 1742;
      else if (highCard1 == 10)
        counter = 1787;
      else if (highCard1 == 9)
        counter = 1832;
      else if (highCard1 == 8)
        counter = 1877;
      else if (highCard1 == 7)
        counter = 1922;
      else if (highCard1 == 6)
        counter = 1967;
      else if (highCard1 == 5)
        counter = 2011;
      else if (highCard1 == 4)
        counter = 2054;
      else if (highCard1 == 3)
        counter = 2097;
      else
        counter = 2140;

      for (int first = highCard1; first >= 2; first--)
        for (int second = 14; second >= 5; second--)
          if (first != second)
            for (int third = second - 1; third >= 4; third--)
              if ((first != third) && (!(first <= 4 && second == 6 && third == 5) //avoid having a 2-3-4-5-6 straight
                                       && !(first == 5 && second == 6 && third == 4)
                                       && !(first == 6 && second == 5 && third == 4)
                                       && !(first == 14 && second == 5 && third == 4) //avoid having a A-2-3-4-5 straight
                                       && !(first == 5 && second == 14 && third == 4)
                                       && !((first == 3 || first == 4) && second == 14 && third == 5)
                                       && !(first == 3 && second >= 5 && third == 4) //avoid impossible case
                                       && !(first == 2 && second == 14 && third == 5)
                                       && !(first == 2 && second >= 5 && third == 4)))
              {
                counter++;

                if (first == highCard1 && second == highCard2 && third == highCard3)
                {
                  handRanking = counter;
                  goto handRankingFound;
                }
              }
    }

    int handRanking2Pairs() // [763, 2184 to 2946]
    {
      int counter;
      switch(highCard1)
      {
        case 14:
          counter = 2183;
          break;
        case 13:
          counter = 2301;
          break;
        case 12:
          counter = 2410;
          break;
        case 11:
          counter = 2509;
          break;
        case 10:
          counter = 2598;
          break;
        case 9:
          counter = 2677;
          break;
        case 8:
          counter = 2746;
          break;
        case 7:
          counter = 2805;
          break;
        case 6:
          counter = 2854;
          break;
        case 5:
          counter = 2892;
          break;
        case 4:
          counter = 2920;
          break;
        default:
          counter = 2938;
          break;
      }

      for (int first = highCard1; first >= 3; first--)
        for (int second = first - 1; second >= 2; second--)
          for (int third = 14; third >= 3; third--)
            if ((third != first && third != second)
            && !((first >= 7 && first <= 13) && second == 2 && (third == 3 || third == 4))//avoid impossible case
            && !(first == 6 && second == 2 && (third == 3 || third == 4 || third == 5))//avoid having a 2-3-4-5-6 straight
            && !(first == 5 && second == 2 && (third == 3 || third == 4 || third == 6))
            && !(first == 4 && second == 2 && (third == 3 || third == 5 || third == 6))
            && !(first == 3 && second == 2 && (third == 4 || third == 5 || third == 6))
            && !(first == 14 && second == 2 && (third >= 3 && third <= 5)))//avoid having a A-2-3-4-5 straight
            //&& !(first == 3 && second == 2 && third == 14))
            {
              counter++;

              if (first == highCard1 && second == highCard2 && third == highCard3)
              {
                handRanking = counter;
                goto handRankingFound;
              }
            }
    }
    int handRanking2Pairs_2() // [763, 2184 to 2946]
    {
      int counter;
      if (highCard1 == 14)
        counter = 2183;
      else if (highCard1 == 13)
        counter = 2301;
      else if (highCard1 == 12)
        counter = 2410;
      else if (highCard1 == 11)
        counter = 2509;
      else if (highCard1 == 10)
        counter = 2598;
      else if (highCard1 == 9)
        counter = 2677;
      else if (highCard1 == 8)
        counter = 2746;
      else if (highCard1 == 7)
        counter = 2805;
      else if (highCard1 == 6)
        counter = 2854;
      else if (highCard1 == 5)
        counter = 2892;
      else if (highCard1 == 4)
        counter = 2920;
      else
        counter = 2938;

      for (int first = highCard1; first >= 3; first--)
        for(int second = first - 1; second >= 2; second--)
          for (int third = 14; third >= 3; third--)
            if ((third != first && third != second)
                && !((first >= 7 && first <= 13) && second == 2 && (third == 3 || third == 4))//avoid impossible case
                && !(first == 6 && second == 2 && (third == 3 || third == 4 || third == 5))//avoid having a 2-3-4-5-6 straight
                && !(first == 5 && second == 2 && (third == 3 || third == 4 || third == 6))
                && !(first == 4 && second == 2 && (third == 3 || third == 5 || third == 6))
                && !(first == 3 && second == 2 && (third == 4 || third == 5 || third == 6))
                && !(first == 14 && second == 2 && (third >= 3 && third <= 5)))//avoid having a A-2-3-4-5 straight
                //&& !(first == 3 && second == 2 && third == 14))
            {
              counter++;

              if (first == highCard1 && second == highCard2 && third == highCard3)
              {
                handRanking = counter;
                goto handRankingFound;
              }
            }
    }
    int handRankingPair() // [1470, 2947 to 4416]
    {
      int counter;
      switch(highCard1)
      {
        case 14:
          counter = 2946;
          break;
        case 13:
          counter = 3058;
          break;
        case 12:
          counter = 3176;
          break;
        case 11:
          counter = 3294;
          break;
        case 10:
          counter = 3412;
          break;
        case 9:
          counter = 3530;
          break;
        case 8:
          counter = 3648;
          break;
        case 7:
          counter = 3766;
          break;
        case 6:
          counter = 3884;
          break;
        case 5:
          counter = 3996;
          break;
        case 4:
          counter = 4101;
          break;
        case 3:
          counter = 4206;
          break;
        default:
          counter = 4311;
          break;
      }

      for (int first = highCard1; first >= 2; first--)
        for (int second = 14; second >= 7; second--)
          if (first != second)
            for (int third = second - 1; third >= 5; third--)
              if (first != third)
                for (int fourth = third - 1; fourth >= 4; fourth--)
                  if ((first != fourth) 
                      && !(first <= 6 && second == 7) //avoid having a 2-3-4-5-6 and 3-4-5-6-7 double straight
                      && !(first == 14 && (second >= 7 && second <= 13) && third == 5 && fourth == 4) //avoid having A-2-3-4-5 straight
                      && !((first >= 6 && first <= 13) && second == 14 && third == 5 && fourth == 4)
                      && !(first == 5 && second == 14 && (third >= 6 && third <= 13) && fourth == 4)
                      && !(first == 4 && second == 14 && (third >= 6 && third <= 13) && fourth == 5)
                      && !((first == 2 || first == 3) && second == 14 && (third >= 6 && third <= 13) && fourth == 5)
                      && !((first == 2 || first == 3) && second == 14 && (third >= 5 && third <= 13) && fourth == 4)
                      && !(first == 6 && second >= 8 && third == 5 && fourth == 4) //avoid having (A-2-3-4-5 or) 2-3-4-5-6 (double) straight
                      && !(first == 5 && second >= 8 && third == 6 && fourth == 4)
                      && !(first <= 4 && second >= 8 && third == 6 && fourth == 5)
                      && !(first <= 3 && (second >= 8 && second <= 13) && (third >= 5 && third < second) && fourth == 4)) //avoid impossible combinations
                  {
                    counter++;

                    if (first == highCard1 && second == highCard2 && third == highCard3 && fourth == highCard4)
                    {
                      handRanking = counter;
                      goto handRankingFound;
                    }
                  }
    }
    int handRankingPair_2() // [1470, 2947 to 4416]
    {
      int counter;
      if (highCard1 == 14)
        counter = 2946;
      else if (highCard1 == 13)
        counter = 3058;
      else if (highCard1 == 12)
        counter = 3176;
      else if (highCard1 == 11)
        counter = 3294;
      else if (highCard1 == 10)
        counter = 3412;
      else if (highCard1 == 9)
        counter = 3530;
      else if (highCard1 == 8)
        counter = 3648;
      else if (highCard1 == 7)
        counter = 3766;
      else if (highCard1 == 6)
        counter = 3884;
      else if (highCard1 == 5)
        counter = 3996;
      else if (highCard1 == 4)
        counter = 4101;
      else if (highCard1 == 3)
        counter = 4206;
      else
        counter = 4311;

      for (int first = highCard1; first >= 2; first--)
        for (int second = 14; second >= 7; second--)
          if (first != second)
            for (int third = second - 1; third >= 5; third--)
              if (first != third)
                for (int fourth = third - 1; fourth >= 4; fourth--)
                  if ((first != fourth)
                  && !(first <= 6 && second == 7) //avoid having a 2-3-4-5-6 and 3-4-5-6-7 double straight
                  && !(first == 14 && (second >= 7 && second <= 13)&& third == 5 && fourth == 4) //avoid having A-2-3-4-5 straight
                  && !((first >= 6 && first <= 13) && second == 14 && third == 5 && fourth == 4)
                  && !(first == 5 && second == 14 && (third >= 6 && third <= 13) && fourth == 4)
                  && !(first == 4 && second == 14 && (third >= 6 && third <= 13) && fourth == 5)
                  && !((first == 2 || first == 3) && second == 14 && (third >= 6 && third <= 13) && fourth == 5)
                  && !((first == 2 || first == 3) && second == 14 && (third >= 5 && third <= 13) && fourth == 4)
                  && !(first == 6 && second >= 8 && third == 5 && fourth == 4) //avoid having (A-2-3-4-5 or) 2-3-4-5-6 (double) straight
                  && !(first == 5 && second >= 8 && third == 6 && fourth == 4)
                  && !(first <= 4 && second >= 8 && third == 6 && fourth == 5)
                  && !(first <= 3 && (second >= 8 && second <= 13) && (third >= 5 && third < second) && fourth == 4))//avoid impossible combinations
                  {
                    counter++;

                    if (first == highCard1 && second == highCard2 && third == highCard3 && fourth == highCard4)
                    {
                      handRanking = counter;
                      goto handRankingFound;
                    }
                  }
    }
    int handRankingHighCard() // [1, 4417 to 4417]
    {
      int handRanking = 4417;
      /*
          int counter;
          if (highCard1 == 14) {counter = 4550;}
          else if (highCard1 == 13) {counter = 5043;}
          else if (highCard1 == 12) {counter = 5372;}
          else if (highCard1 == 11) {counter = 5581;}
          else if (highCard1 == 10) {counter = 5706;}
          else if (highCard1 == 9) {counter = 5775;}
          else if (highCard1 == 8) {counter = 5809;}
          else {counter = 5823;}

          for (int first = highCard1; first >= 2; first--)
              for(int second = first - 1; second >= 5; second--)
                  if (first != second)
                      for (int third = second - 1; third >= 4; third--)
                          if (first != third)
                              for (int fourth = third - 1; fourth >= 3; fourth--)
                                  if (first != fourth)
                                      for (int fifth = fourth - 1; fifth >= 2; fifth--) // since the last high card (subcategory of this hand) is at least a two, I don't need to check for (first == 5 && fifth == 1)
                                          if (first != fifth)
                                              if (!( (first == 14 && fifth == 10) || (first == 13 && fifth == 9) // avoid having any kind of straight
                                                    || (first == 12 && fifth == 8) || (first == 11 && fifth == 7)
                                                    || (first == 10 && fifth == 6) || (first == 9 && fifth == 5)
                                                    || (first == 8 && fifth == 4) || (first == 7 && fifth == 3)
                                                    || (first == 6 && fifth == 2) || (first == 14 && second == 5)))
                                              {
                                                  counter++;

                                                  if (first == highCard1 && second == highCard2 && third == highCard3 && fourth == highCard4 && fifth == highCard5)
                                                  {
                                                      handRanking = counter;
                                                      goto handRankingFound;
                                                  }
                                              }
      */
      return handRanking;
    }
};




















int handValueCalculation_NEW_3(Data data)
{
  //DO 1ST BECAUSE IT'S SUPER FAST AND CAN RULE OUT MANY POSSIBLE HANDS WHEN IT FINDS A FLUSH
  data.checkForFlush();

  //DO 2ND BECAUSE IT CAN TEST FOR A STRAIGHT AND ALMOST EVERY OTHER TYPE OF HAND
  // used to determine if you got a 4K, FH, 3K, 2P, P, or no pair (used in conditionals at varying stages of this method)
  // Can also rule out all forms of F and S since they are mutually exlusive with 4K and FH
  data.testStraightAndRepeatedRanks();

  //Quickly rule out anything that isn't a flush, straight, or straight/royal flush
  data.checkForStraightAndFlush();

  //Can be used to quickly determine the first high card for a 4K, FH, 3K, 2P, and P
  //Don't use this to determine the high card for any other hand
  data.checkForMatchedRanks();

  if (largestMatch == 4) //If it's a 4oaK, there is no way it could be anything higher
  {
    goto is4K;
  }
  else if (largestMatch == 3 && numberOfRepeats >= 2) //Possible to have two triples, or two doubles with a triple
  {
    goto isFH;
  }
  else if (largestMatch == 3 && numberOfRepeats == 1)
  {
    goto is3K;
  }
  else if (numberOfRepeats >= 2)
  {
    goto is2P;
  }
  else if (numberOfRepeats == 1)
  {
    goto isPair;
  }
  else
  {
    hand = 10;
    goto handDecided;
  }

  /* DETERMINE HAND */

  //AT LEAST A STRAIGHT FLUSH
  atLeastStraightFlush:;

  data.checkIfRoyalFlush();
  data.checkIfStraightFlush();
  data.checkIf4OfAKind();

 

  /* DETERMINE RANK OF HAND */


  
  

  handRankingFound:;
  return handRanking;
}

int handValueCalculation_NEW_2_DIVIDED(Data data)
{
  data.setMatrixCounts();
  data.setRankCounts();

  data.quickCheckForFlush();

  //DO 2ND BECAUSE IT CAN TEST FOR A STRAIGHT AND ALMOST EVERY OTHER TYPE OF HAND
  data.testStraightAndRepeatedRanks();

  data.checkForStraightAndFlush();

  //Can be used to quickly determine the first high card for a 4K, FH, 3K, 2P, and P
  //Don't use this to determine the high card for any other hand
  data.checkForMatchedRanks();


  /* DETERMINE HAND */

  /* DETERMINE RANK OF HAND */

  handRankingFound:;
  return handRanking;
}

int handValueCalculation_NEW_2(Data data)
{
  data.setMatrixCounts();
  data.setRankCounts();

  //DO 1ST BECAUSE IT'S SUPER FAST AND CAN RULE OUT MANY POSSIBLE HANDS WHEN IT FINDS A FLUSH
  data.checkForFlush();

  //DO 2ND BECAUSE IT CAN TEST FOR A STRAIGHT AND ALMOST EVERY OTHER TYPE OF HAND
  data.testStraightAndRepeatedRanks();

  //Quickly rule out anything that isn't a flush, straight, or straight/royal flush
  int options1 = checkForStraightAndFlush(cards, data);

  //USE THIS SWITCH CASE INSTEAD OF GOTOs
  switch (options1)
  {
  case IS_STRAIGHT:
    dothis();
    andthis();
    break;
  case IS_FLUSH:
    break;
  case IS_STRAIGHT_FLUSH:
    break;
  case AT_LEAST_STRAIGHT_FLUSH:
    break;
  default: //NO_MATCHES
    break;
  }

  //Can be used to quickly determine the first high card for a 4K, FH, 3K, 2P, and P
  //Don't use this to determine the high card for any other hand
  int options2 = checkForMatchedRanks(data);

  //USE THIS SWITCH CASE INSTEAD OF GOTOs
  switch (options2)
  {
  case IS_4_OF_A_KIND:
    dothis();
    andthis();
    break;
  case IS_FULL_HOUSE:
    break;
  case IS_3_OF_A_KIND:
    break;
  case IS_2_PAIR:
    break;
  case IS_PAIR:
    break;
  default: //NO_STRAIGHT_NOR_FLUSH
    break;
  }

  /* DETERMINE HAND */

  checkIfRoyalFlush();
  checkIfStraightFlush();
  checkIf4OfAKind();
  checkIfFullHouse();
  checkIfFlush();
  checkIfStraight();
  checkIf3OfAKind();
  checkIf2Pairs();
  checkIfPair();

  /* DETERMINE HIGH CARDS */

  // checkIfHighCard(data);

  /* DETERMINE RANK OF HAND */
  handDecided:;

  switch (hand)
  {
  case IS_ROYAL_FLUSH:
    handRankingRoyalFlush(); // [1, 0 to 0]
  case IS_STRAIGHT_FLUSH:
    handRankingStraightFlush(); // [9, 1 to 9]
  case IS_4_OF_A_KIND:
    handRanking4OfAKind(); // [156, 10 to 165]
  case IS_FULL_HOUSE:
    handRankingFullHouse(); // [156, 166 to 321]
  case IS_FLUSH:
    handRankingFlush(); // [1277, 322 to 1598]
  case IS_STRAIGHT:
    handRankingStraight(); // [10, 1599 to 1608]
  case IS_3_OF_A_KIND:
    handRanking3OfAKind(); // [575, 1609 to 2183]
  case IS_2_PAIR:
    handRanking2Pairs(); // [763, 2184 to 2946]
  case IS_PAIR:
    handRankingPair(); // [1470, 2947 to 4416]
  case IS_HIGH_CARD:
    handRankingHighCard(); // [1, 4417 to 4417]
  }

  return handRanking;
}

//for some reason, having two identical copies of a handValueCalculation() method will slightly (~1%) increase the speed of the second one
int handValueCalculation_NEW(Data data)
{
  data.setMatrixCounts();
  data.setRankCounts();

  /*
    //NEW IDEAS TO TEST OUT FROM 2017


  //DO 3RD
    if (you at least have a straight)
        This hand is at least a straight, and possibly a straight/royal flush
        This hand is definitely not a 4-of a kind or full house


    if (you have a flush and a straight)
        // Very quick check to rule out all straight/royal flushes
        // Most flushes lack at least one high card, but this check does not rule out cases where you have a non-overlapping straight and flush
        if (aces == 0 || kings == 0 || queens == 0 || jacks == 0 || tens == 0)
            definitely not a straight/royal flush, therefore it is definitely a flush
        else if ((s1 == s5 && r5 - r1 == 4) || (s2 == s6 && r6 - r2 == 4) || (s3 == s7 && r7 - r3 == 4))
            definitely at least a straight flush, covers all straight flush cases, could still be a royal flush


    New: Now that I have a fast way to find a flush, add a condition so that the hand must contain both a flush AND a straight before checking if there's a straight/royal flush

  */

  /* DETERMINE HAND */

  int containsStraight = 0; //if 1 / true, 4oaK and FH are impossible
  int containsFlush = 0;    //if 1 / true, 4oaK and FH are impossible

  //DO 1ST BECAUSE IT'S SUPER FAST AND CAN RULE OUT MANY POSSIBLE HANDS WHEN IT FINDS A FLUSH
  if (data.checkForFlush())
  {
    containsFlush = 1;
    }

    // Test if it's NOT a straight
    if (checkIfNotStraight())
      hand = 10;
    else
    {
        hand = 6;
        containsStraight = 1;
    }

    //IS AT LEAST A STRAIGHT and a flush
    if (containsStraight == 1 && containsFlush == 1)
    {
        //ROYAL FLUSH
        if (data.checkIfRoyalFlush_2())
        {
            hand = 1;
            goto handDecided;
        }

        //STRAIGHT FLUSH
        data.checkIfStraightFlush_2();
    }

    if (containsStraight == 0 && containsFlush == 0)
    {
      data.checkIf4OfAKind_2();

        //FULL HOUSE
      data.checkIfFullHouse();
    }

    //FLUSH
    if (containsFlush == 1)
    {
        data.checkIfFlush();
    }

    if (containsStraight == 1) //STRAIGHT - Check 1 high card
    {
      data.checkIfStraight_2();
    }

    //THREE OF A KIND
    if ()
    {
      data.checkIf3OfAKind_2();
    }

    //TWO PAIRS
    if ()
    {
      data.checkIf2Pairs_2();
    }

    //PAIR
    if (data.checkIfPair_2())
    {
        data.highCardPair();
    }

    //HIGH CARD - Check 5 high cards
    else
    {

    }



    /* DETERMINE RANK OF HAND */
    handRankingRoyalFlush(); // [1, 0 to 0]
    handRankingStraightFlush(); // [9, 1 to 9]


}

/* Used by every calculateHand() function to calculate the array of all 4417 possible hands for any particular 7 card hand containing the necessary 2/3/4/5/6 card hand (when applicable), but omits all HC */
int handValueCalculation(Data data)
{
  data.setMatrixCounts();
  data.setRankCounts();

  
  data.checkForFlush();

  //DO 2ND BECAUSE IT CAN TEST FOR A STRAIGHT AND ALMOST EVERY OTHER TYPE OF HAND
    // used to determine if you got a 4oaK, full house, 3oaK, 2 pair, pair, or no pair (used in conditionals at varying stages of this method)
    // Can also rule out all forms of flushes and straights:

  data.testStraightAndRepeatedRanks_4();

  //can be used to quickly determine the first high card for a 4oaK, full house, 3oaK, 2 pair, and pair
  //Don't use this to determine the high card for any other hand
  data.checkForMatchedRanks();



  //DO 3RD
    // if (you at least have a straight)
    //     This hand is at least a straight, and possibly a straight/royal flush
    //     This hand is definitely not a 4-of a kind or full house


    // if (you have a flush and a straight)
    //     // Very quick check to rule out all straight/royal flushes
    //     // Most flushes lack at least one high card, but this check does not rule out cases where you have a non-overlapping straight and flush
    //     if (aces == 0 || kings == 0 || queens == 0 || jacks == 0 || tens == 0)
    //         definitely not a straight/royal flush, therefore it is definitely a flush
    //     else if ((s1 == s5 && r5 - r1 == 4) || (s2 == s6 && r6 - r2 == 4) || (s3 == s7 && r7 - r3 == 4))
    //         definitely at least a straight flush, covers all straight flush cases, could still be a royal flush


    // New: Now that I have a fast way to find a flush, add a condition so that the hand must contain 
    // both a flush AND a straight before checking if there's a straight/royal flush

  /* DETERMINE HAND */

  // Test if it's NOT a straight
  if (!quickCheckForStraight())
    hand = 10; //HIGH CARD UNLESS IT PASSES ONE OF THE FOLLOWING CONDITIONS
  else
    hand = 6; //STRAIGHT UNLESS IT PASSES ONE OF THE FOLLOWING HIGHER CONDITIONS

  //IS AT LEAST A STRAIGHT
  if (hand == 6)
  {
    //ROYAL FLUSH
    //FULL HOUSE
    //FLUSH
    //TWO PAIRS
    //PAIR
    //HIGH CARD - Check 5 high cards
    }

    handDecided:;
    /* DETERMINE RANK OF HAND */

    //THREE OF A KIND // [575, 1609 to 2183]
    handRankingFound:;
    return handRanking;
}






















//I can create a variation of this method to verify that the 3 different hand value calculation method give the same results for any given card combination
void verifyCardCounters(int* cardCounter, int counterTotal, int comboLength)
{
    int counterRF, counterSF, counter4K, counterFH, counterF, counterS, counter3K, counter2P, counterP, counterHC;
    counterRF = counterSF = counter4K = counterFH = counterF = counterS = counter3K = counter2P = counterP = counterHC = 0;

    //FOR DEBUGGING PURPOSES
    for (int handRanking = 0; handRanking < 4417; handRanking++)
    {
        if (handRanking >= 2947 && handRanking <= 4416) { counterP += cardCounter[handRanking]; }
        else if (handRanking >= 2184 && handRanking <= 2946) { counter2P += cardCounter[handRanking]; }
        else if (handRanking >= 1609 && handRanking <= 2183) { counter3K += cardCounter[handRanking]; }
        else if (handRanking >= 1599 && handRanking <= 1608) { counterS += cardCounter[handRanking]; }
        else if (handRanking >= 322 && handRanking <= 1598) { counterF += cardCounter[handRanking]; }
        else if (handRanking >= 166 && handRanking <= 321) { counterFH += cardCounter[handRanking]; }
        else if (handRanking >= 10 && handRanking <= 165) { counter4K += cardCounter[handRanking]; }
        else if (handRanking >= 1 && handRanking <= 9) { counterSF += cardCounter[handRanking]; }
        else if (handRanking == 0) { counterRF += cardCounter[handRanking]; }
    }

    printf ("Inner-L cycles: %*d / 133784560 \n", 9, counterTotal);
    printf ("      RF count: %*d / 4324 at most\n", 9, counterRF);
    printf ("      SF count: %*d / 37260 at most\n", 9, counterSF);
    printf ("      4K count: %*d / 224848 at most\n", 9, counter4K);
    printf ("      FH count: %*d / 3473184 at most\n", 9, counterFH);
    printf ("      Fl count: %*d / 4047644 at most\n", 9, counterF);
    printf ("      St count: %*d / 6180020 at most\n", 9, counterS);
    printf ("      3K count: %*d / 6461620 at most\n", 9, counter3K);
    printf ("      2P count: %*d / 31433400 at most\n", 9, counter2P);
    printf ("       P count: %*d / 58627800 at most\n", 9, counterP);
    //printf ("      HC count: %*d / 23294460 at most\n\n", 9, counterHC);

    if (comboLength == 7 && counterTotal == 133784560 && counterRF == 4324 && counterSF == 37260 && counter4K == 224848 && counterFH == 3473184 && counterF == 4047644 && counterS == 6180020 && counter3K == 6461620 && counter2P == 31433400 && counterP == 58627800)
        printf("All values are good!\n");
    else if (comboLength == 6)
        printf("All values are good!\n");
    else
        printf("Something is fucked up!\n");
    printf("\a");
}






bool irrelevantOld(int cardMatrix[][14], int suitsAndRanks[], int currentMatches)
{
  return cardMatrix[suitsAndRanks[0]][suitsAndRanks[1]]
       + cardMatrix[suitsAndRanks[2]][suitsAndRanks[3]]
       + cardMatrix[suitsAndRanks[4]][suitsAndRanks[5]]
       + cardMatrix[suitsAndRanks[6]][suitsAndRanks[7]]
       + cardMatrix[suitsAndRanks[8]][suitsAndRanks[9]] <= currentMatches;
}

int setNextRank(int suit1, int suit2, int rank)
{
  if (suit1 == suit2)
  {
    return ++rank;
  }
  else
  {
    return 1;
  }
}






/* Used to calculate the value of every complete 7 card hand */
/* Meant to debug / test handValueCalculation() */
void calculateAllHands_7()
{
    int handRanking;

    int counterTotal, counterRF, counterSF, counter4K, counterFH, counterF, counterS, counter3K, counter2P, counterP, counterHC;
    int setr2, setr3, setr4, setr5, setr6, setr7;

    /* START OF INNER LOOP */
    counterTotal = counterRF = counterSF = counter4K = counterFH = counterF = counterS = counter3K = counter2P = counterP = counterHC = 0;
    int cardCounter[4417] = {0};

    for (int s1 =  1; s1 <= 4; s1++){
        for (int r1 = 1; r1 <= 13; r1++){                   printf("%d %d \n\n", s1, r1);
            for (int s2 = s1; s2 <= 4; s2++){
                if (s1 == s2) {setr2 = r1 + 1;} else {setr2 = 1;}
                for (int r2 = setr2; r2 <= 13; r2++){
                    for (int s3 = s2; s3 <= 4; s3++){
                        if (s2 == s3) {setr3 = r2 + 1;} else {setr3 = 1;}
                        for (int r3 = setr3; r3 <= 13; r3++){
                            for (int s4 = s3; s4 <= 4; s4++){
                                if (s3 == s4) {setr4 = r3 + 1;} else {setr4 = 1;}
                                for (int r4 = setr4; r4 <= 13; r4++){
                                    for (int s5 = s4; s5 <= 4; s5++){
                                        if (s4 == s5) {setr5 = r4 + 1;} else {setr5 = 1;}
                                        for (int r5 = setr5; r5 <= 13; r5++){
                                            for (int s6 = s5; s6 <= 4; s6++){
                                                if (s5 == s6) {setr6 = r5 + 1;} else {setr6 = 1;}
                                                for (int r6 = setr6; r6 <= 13; r6++){
                                                    for (int s7 = s6; s7 <= 4; s7++){
                                                        if (s6 == s7) {setr7 = r6 + 1;} else {setr7 = 1;}
                                                        for (int r7 = setr7; r7 <= 13; r7++){
                                                          
                                                            Cards cards(s1, r1, s2, r2, s3, r3, s4, r4, s5, r5, s6, r6, s7, r7);
                                                            //handRanking = handValueCalculation(cards);
                                                            handRanking = handValueCalculation_NEW(cards);
                                                            //handRanking = handValueCalculation_NEW_2(cards);
                                                            if (handRanking != 4417)
                                                                cardCounter[handRanking] += 1;

                                                            counterTotal++;

                                                        }}}}}}}}}}}}}}

    verifyCardCounters(cardCounter, counterTotal, 7);

    /*ofstream outFile;
    outFile.open("text.txt");

    outFile << "if (";

    for (int a = 2184; a <= 2975; a++)
        if (cardCounter[a] == 0)
        {
            outFile << "count == " << a << " || " ;
        }
    outFile <<")";
    outFile.close();*/


    /*for (int a = 0; a <= 4416; a++)
        if (cardCounter[a] == 0)
        {
            cout << a << " ";
        }*/


    //printf("\n\n\n%d, %d, %d", universalCounter_1, universalCounter_2, universalCounter_3);
    //printf("\n\n\n%d, %d, %d", universalCounter_4, universalCounter_5, universalCounter_6);
}
/* Used to calculate every 6 card hand to build the 6cards database */
void calculateAllHands_6()
{
    // With extra break at the end
    // ????s full
    // 2318s without checking hand values - use MAX-CONDITION
    // 2336s without checking hand values - use MAX-CONDITION
    // 1169s without checking hand values - use INNER-MAX-CONDITION
    // 1215s without checking hand values - use INNER-MAX-CONDITION
    // 1234s without checking hand values - use both

    int handRanking;

    int counterRF, counterSF, counter4K, counterFH, counterF, counterS, counter3K, counter2P, counterP, counterHC;
    int cardMatrix[5][14] = {0};
    int setr2, setr3, setr4, setr5, setr6, setr7, setcR2, setcR3, setcR4, setcR5, setcR6;


    //2cards
    for (int cS1 = 1; cS1 <=  4; cS1++){
        for (int cR1 = 1; cR1 <= 13; cR1++){                   printf("%d %d \n\n", cS1, cR1);
            for (int cS2 = cS1; cS2 <= 4; cS2++){
                if (cS1 == cS2) {setcR2 = cR1 + 1;} else {setcR2 = 1;}
                for (int cR2 = setcR2; cR2 <= 13; cR2++){
                    //3cards
                    for (int cS3 = cS2; cS3 <= 4; cS3++){
                        if (cS2 == cS3) {setcR3 = cR2 + 1;} else {setcR3 = 1;}
                        for (int cR3 = setcR3; cR3 <= 13; cR3++){
                            //4cards
                            for (int cS4 = cS3; cS4 <= 4; cS4++){
                                if (cS3 == cS4) {setcR4 = cR3 + 1;} else {setcR4 = 1;}
                                for (int cR4 = setcR4; cR4 <= 13; cR4++){
                                    //5cards
                                    for (int cS5 = cS4; cS5 <= 4; cS5++){
                                        if (cS4 == cS5) {setcR5 = cR4 + 1;} else {setcR5 = 1;}
                                        for (int cR5 = setcR5; cR5 <= 13; cR5++){
                                            //6cards
                                            for (int cS6 = cS5; cS6 <= 4; cS6++){
                                                if (cS5 == cS6) {setcR6 = cR5 + 1;} else {setcR6 = 1;}
                                                for (int cR6 = setcR6; cR6 <= 13; cR6++){


                                                    /* START OF INNER LOOP */
                                                    int counterTotal = counterRF = counterSF = counter4K = counterFH = counterF = counterS = counter3K = counter2P = counterP = counterHC = 0;
                                                    int cardCounter[4417] = {0};

                                                    for (int s1 =  1; s1 <= 4; s1++){
                                                        for (int r1 = 1; r1 <= 13; r1++){
                                                            cardMatrix[s1][r1] = 1;
                                                            for (int s2 = s1; s2 <= 4; s2++){
                                                                if (s1 == s2) {setr2 = r1 + 1;} else {setr2 = 1;}
                                                                for (int r2 = setr2; r2 <= 13; r2++){
                                                                    cardMatrix[s2][r2] = 1;
                                                                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] + cardMatrix[ cS6 ][ cR6 ] == 0) { goto r2Skip;} /* IRRELEVANT: 6cards */
                                                                    for (int s3 = s2; s3 <= 4; s3++){
                                                                        if (s2 == s3) {setr3 = r2 + 1;} else {setr3 = 1;}
                                                                        for (int r3 = setr3; r3 <= 13; r3++){
                                                                            cardMatrix[s3][r3] = 1;
                                                                            if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] + cardMatrix[ cS6 ][ cR6 ] <= 1) { goto r3Skip;} /* IRRELEVANT: 6cards */
                                                                            for (int s4 = s3; s4 <= 4; s4++){
                                                                                if (s3 == s4) {setr4 = r3 + 1;} else {setr4 = 1;}
                                                                                for (int r4 = setr4; r4 <= 13; r4++){
                                                                                    cardMatrix[s4][r4] = 1;
                                                                                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] + cardMatrix[ cS6 ][ cR6 ] <= 2) { goto r4Skip;} /* IRRELEVANT: 6cards */
                                                                                    for (int s5 = s4; s5 <= 4; s5++){
                                                                                        if (s4 == s5) {setr5 = r4 + 1;} else {setr5 = 1;}
                                                                                        for (int r5 = setr5; r5 <= 13; r5++){
                                                                                            cardMatrix[s5][r5] = 1;
                                                                                            if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] + cardMatrix[ cS6 ][ cR6 ] <= 3) { goto r5Skip;} /* IRRELEVANT: 6cards */
                                                                                            for (int s6 = s5; s6 <= 4; s6++){
                                                                                                if (s5 == s6) {setr6 = r5 + 1;} else {setr6 = 1;}
                                                                                                for (int r6 = setr6; r6 <= 13; r6++){
                                                                                                    cardMatrix[s6][r6] = 1;
                                                                                                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] + cardMatrix[ cS6 ][ cR6 ] <= 4) { goto r6Skip;} /* IRRELEVANT: 6cards */
                                                                                                    for (int s7 = s6; s7 <= 4; s7++){
                                                                                                        if (s6 == s7) {setr7 = r6 + 1;} else {setr7 = 1;}
                                                                                                        for (int r7 = setr7; r7 <= 13; r7++){
                                                                                                            cardMatrix[s7][r7] = 1;
                                                                                                            if (cardMatrix[ cS1 ][ cR1 ] == 0 || cardMatrix[ cS2 ][ cR2 ] == 0 || cardMatrix[ cS3 ][ cR3 ] == 0 || cardMatrix[ cS4 ][ cR4 ] == 0 || cardMatrix[ cS5 ][ cR5 ] == 0 || cardMatrix[ cS6 ][ cR6 ] == 0) { goto r7Skip;} /* IRRELEVANT: 6cards */

                                                                                                            Cards cards(s1, r1, s2, r2, s3, r3, s4, r4, s5, r5, s6, r6, s7, r7);
                                                                                                            handRanking = handValueCalculation(cards);
                                                                                                            if (handRanking != 4417)
                                                                                                                cardCounter[handRanking] += 1;

                                                                                                            counterTotal++;

                                                                                                            if (counterTotal == 46) /* INNER-MAX-CONDITION: 6cards*/
                                                                                                                goto nextCombo;

                                                                                                            r7Skip:; cardMatrix[s7][r7] = 0;}}
                                                                                                    r6Skip:; cardMatrix[s6][r6] = 0;}}
                                                                                            r5Skip:; cardMatrix[s5][r5] = 0;}}
                                                                                    r4Skip:; cardMatrix[s4][r4] = 0;}}
                                                                            r3Skip:; cardMatrix[s3][r3] = 0;}}
                                                                    r2Skip:; cardMatrix[s2][r2] = 0;}}
                                                            cardMatrix[s1][r1] = 0;}}

                                                    nextCombo:;

                                                    //verifyCardCounters(int* cardCounter, int counterTotal, 6)

                                                }}}}}}}}}}break;}}
    cout << "\a";
}
/* Used to calculate every 5 card hand to build the 5cards database */
void calculateAllHands_5_GR(int xHandRanks[4417], int cS1, int cR1, int cS2, int cR2, int cS3, int cR3, int cS4, int cR4, int cS5, int cR5)
{
    // With extra break at the end // For the sake of speeding up testing, I added a break statement at the end of the cR1 loop so that I only test the first rank of each suit
    // None of these check hand values for the sake of testing purely the speed of the inner loop

    // IRRELEVANT checks are about 2-3% faster with "goto" than "continue"
    // INNER-MAX-CONDITION is about 25% faster than MAX-CONDITION, but using both is slighter slower than INNER-MAX-CONDITIONS alone.
    // I suspect the benefit of using INNER-MAX-CONDITION over MAX-CONDITION is greater for larger hands, and it was about 1% slower when I tested this on calculateAllHands_2()

    // Feb 24
    // 74 - use New IRRELEVANT CONDITION        only
    // 314 - use Old IRRELEVANT CONDITION       only
    // 1.77 - use GUARANTEED-RELEVANCE CONDITION    only
    // 2.94 - use GUARANTEED-RELEVANCE CONDITION & New IRRELEVANT CONDITION
    //  - use GUARANTEED-RELEVANCE CONDITION & Old IRRELEVANT CONDITION

    int handRanking;

    int cardMatrix[5][14] = {0};
    int counterTotal = 0;
    int setr2, setr3, setr4, setr5, setr6, setr7, setcR2, setcR3, setcR4, setcR5;
    int s1, s2, s3, s4, s5, s6, s7;
    int r1, r2, r3, r4, r5, r6, r7;

    //2cards
    /*for (int cS1 = 1; cS1 <=  4; cS1++){
    for (int cR1 = 1; cR1 <= 13; cR1++){                   //printf("%d%d \n\n", cS1, cR1);
    for (int cS2 = cS1; cS2 <= 4; cS2++){
        if (cS1 == cS2) {setcR2 = cR1 + 1;} else {setcR2 = 1;}
    for (int cR2 = setcR2; cR2 <= 13; cR2++){                   //printf("%d%d %d%d\n\n", cS1, cR1, cS2, cR2);
    //3cards
    for (int cS3 = cS2; cS3 <= 4; cS3++){
        if (cS2 == cS3) {setcR3 = cR2 + 1;} else {setcR3 = 1;}
    for (int cR3 = setcR3; cR3 <= 13; cR3++){
    //4cards
    for (int cS4 = cS3; cS4 <= 4; cS4++){
        if (cS3 == cS4) {setcR4 = cR3 + 1;} else {setcR4 = 1;}
    for (int cR4 = setcR4; cR4 <= 13; cR4++){
    //5cards
    for (int cS5 = cS4; cS5 <= 4; cS5++){
        if (cS4 == cS5) {setcR5 = cR4 + 1;} else {setcR5 = 1;}
    for (int cR5 = setcR5; cR5 <= 13; cR5++){                   //printf("%d%d %d%d %d%d %d%d %d%d\n\n", cS1, cR1, cS2, cR2, cS3, cR3, cS4, cR4, cS5, cR5);*/


    /* START OF INNER LOOP */
    int cardCounter[4417] = {0};

    for (s1 =  1; s1 <= 4; s1++)
    {
        for (r1 = 1; r1 <= 13; r1++)
        {
            cardMatrix[s1][r1] = 1;
            for (s2 = s1; s2 <= 4; s2++)
            {
                if (s1 == s2) {setr2 = r1 + 1;} else {setr2 = 1;}
                for (r2 = setr2; r2 <= 13; r2++)
                {
                    cardMatrix[s2][r2] = 1;
                    for (s3 = s2; s3 <= 4; s3++)
                    {
                        if (s2 == s3) {setr3 = r2 + 1;} else {setr3 = 1;}
                        for (r3 = setr3; r3 <= 13; r3++)
                        {
                            cardMatrix[s3][r3] = 1;
                            if (s1 != cS1 && r1 != cR1 && s2 != cS1 && r2 != cR1) {cardMatrix[s3][r3] = 0;   s3 = cS1; r3 = cR1;   cardMatrix[s3][r3] = 1;} /* GUARANTEED-RELEVANCE: 5cards */
                            //if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] == 0) { goto r3Skip;} /* IRRELEVANT: 5cards */
                            //if (cardMatrix[ cS1 ][ cR1 ] == 0) { goto r3Skip;} /* IRRELEVANT: 5cards */
                            for (s4 = s3; s4 <= 4; s4++)
                            {
                                if (s3 == s4) {setr4 = r3 + 1;} else {setr4 = 1;}
                                for (r4 = setr4; r4 <= 13; r4++)
                                {
                                    cardMatrix[s4][r4] = 1;
                                    if (s3 == cS1 && r3 == cR1) {cardMatrix[s4][r4] = 0;   s4 = cS2; r4 = cR2;   cardMatrix[s4][r4] = 1;} /* GUARANTEED-RELEVANCE: 5cards */
                                    //if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] <= 1) { goto r4Skip;} /* IRRELEVANT: 5cards */
                                    //if (cardMatrix[ cS2 ][ cR2 ] == 0) { goto r4Skip;} /* IRRELEVANT: 5cards */
                                    for (s5 = s4; s5 <= 4; s5++)
                                    {
                                        if (s4 == s5) {setr5 = r4 + 1;} else {setr5 = 1;}
                                        for (r5 = setr5; r5 <= 13; r5++)
                                        {
                                            cardMatrix[s5][r5] = 1;
                                            if (s4 == cS2 && r4 == cR2) {cardMatrix[s5][r5] = 0;   s5 = cS3; r5 = cR3;   cardMatrix[s5][r5] = 1;} /* GUARANTEED-RELEVANCE: 5cards */
                                            //if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] <= 2) { goto r5Skip;} /* IRRELEVANT: 5cards */
                                            //if (cardMatrix[ cS3 ][ cR3 ] == 0) { goto r5Skip;} /* IRRELEVANT: 5cards */
                                            for (s6 = s5; s6 <= 4; s6++)
                                            {
                                                if (s5 == s6) {setr6 = r5 + 1;} else {setr6 = 1;}
                                                for (r6 = setr6; r6 <= 13; r6++)
                                                {
                                                    cardMatrix[s6][r6] = 1;
                                                    if (s5 == cS3 && r5 == cR3) {cardMatrix[s6][r6] = 0;   s6 = cS4; r6 = cR4;   cardMatrix[s6][r6] = 1;} /* GUARANTEED-RELEVANCE: 5cards */
                                                    //if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] <= 3) { goto r6Skip;} /* IRRELEVANT: 5cards */
                                                    //if (cardMatrix[ cS4 ][ cR4 ] == 0) { goto r6Skip;} /* IRRELEVANT: 5cards */
                                                    for (s7 = s6; s7 <= 4; s7++)
                                                    {
                                                        if (s6 == s7) {setr7 = r6 + 1;} else {setr7 = 1;}
                                                        for (r7 = setr7; r7 <= 13; r7++)
                                                        {
                                                            cardMatrix[s7][r7] = 1;
                                                            if (s6 == cS4 && r6 == cR4) {cardMatrix[s7][r7] = 0;   s7 = cS5; r7 = cR5;   cardMatrix[s7][r7] = 1;} /* GUARANTEED-RELEVANCE: 5cards */
                                                            if (cardMatrix[ cS1 ][ cR1 ] == 0 || cardMatrix[ cS2 ][ cR2 ] == 0 || cardMatrix[ cS3 ][ cR3 ] == 0 || cardMatrix[ cS4 ][ cR4 ] == 0 || cardMatrix[ cS5 ][ cR5 ] == 0) { goto r7Skip;} /* IRRELEVANT: 5cards */
                                                            //if (cardMatrix[ cS5 ][ cR5 ] == 0) { goto r7Skip;} /* IRRELEVANT: 5cards */

                                                            Cards cards(s1, r1, s2, r2, s3, r3, s4, r4, s5, r5, s6, r6, s7, r7);
                                                            handRanking = handValueCalculation_NEW(cards);
                                                            if (handRanking != 4417)
                                                                cardCounter[handRanking] += 1;

                                                            counterTotal++;

                                                            if (counterTotal == 1081) /* INNER-MAX-CONDITION: 5cards*/
                                                                goto nextCombo;

                                                            r7Skip:; cardMatrix[s7][r7] = 0;
                                                        }
                                                    }
                                                    r6Skip:; cardMatrix[s6][r6] = 0;
                                                }
                                            }
                                            r5Skip:; cardMatrix[s5][r5] = 0;
                                        }
                                    }
                                    r4Skip:; cardMatrix[s4][r4] = 0;
                                }
                            }
                            r3Skip:; cardMatrix[s3][r3] = 0;
                        }
                    }
                    cardMatrix[s2][r2] = 0;
                }
            }
            cardMatrix[s1][r1] = 0;
        }
    }

    nextCombo:;

    //}}}}}}}}break;}}
    //printf("\a");

    for (int i = 0; i < 4417; i++)
        xHandRanks[i] = cardCounter [i];
}
void calculateAllHands_5_NEW_IRR(int xHandRanks[4417], int cS1, int cR1, int cS2, int cR2, int cS3, int cR3, int cS4, int cR4, int cS5, int cR5)
{
    // With extra break at the end // For the sake of speeding up testing, I added a break statement at the end of the cR1 loop so that I only test the first rank of each suit
    // None of these check hand values for the sake of testing purely the speed of the inner loop

    // IRRELEVANT checks are about 2-3% faster with "goto" than "continue"
    // INNER-MAX-CONDITION is about 25% faster than MAX-CONDITION, but using both is slighter slower than INNER-MAX-CONDITIONS alone.
    // I suspect the benefit of using INNER-MAX-CONDITION over MAX-CONDITION is greater for larger hands, and it was about 1% slower when I tested this on calculateAllHands_2()

    // Feb 24
    // 74 - use New IRRELEVANT CONDITION        only
    // 314 - use Old IRRELEVANT CONDITION       only
    // 1.77 - use GUARANTEED-RELEVANCE CONDITION    only
    // 2.94 - use GUARANTEED-RELEVANCE CONDITION & New IRRELEVANT CONDITION
    //  - use GUARANTEED-RELEVANCE CONDITION & Old IRRELEVANT CONDITION

    int handRanking;

    int cardMatrix[5][14] = {0};
    int counterTotal = 0;
    int setr2, setr3, setr4, setr5, setr6, setr7, setcR2, setcR3, setcR4, setcR5;
    int s1, s2, s3, s4, s5, s6, s7;
    int r1, r2, r3, r4, r5, r6, r7;

    //2cards
    /*for (int cS1 = 1; cS1 <=  4; cS1++){
    for (int cR1 = 1; cR1 <= 13; cR1++){                   //printf("%d%d \n\n", cS1, cR1);
    for (int cS2 = cS1; cS2 <= 4; cS2++){
        if (cS1 == cS2) {setcR2 = cR1 + 1;} else {setcR2 = 1;}
    for (int cR2 = setcR2; cR2 <= 13; cR2++){                   //printf("%d%d %d%d\n\n", cS1, cR1, cS2, cR2);
    //3cards
    for (int cS3 = cS2; cS3 <= 4; cS3++){
        if (cS2 == cS3) {setcR3 = cR2 + 1;} else {setcR3 = 1;}
    for (int cR3 = setcR3; cR3 <= 13; cR3++){
    //4cards
    for (int cS4 = cS3; cS4 <= 4; cS4++){
        if (cS3 == cS4) {setcR4 = cR3 + 1;} else {setcR4 = 1;}
    for (int cR4 = setcR4; cR4 <= 13; cR4++){
    //5cards
    for (int cS5 = cS4; cS5 <= 4; cS5++){
        if (cS4 == cS5) {setcR5 = cR4 + 1;} else {setcR5 = 1;}
    for (int cR5 = setcR5; cR5 <= 13; cR5++){                   //printf("%d%d %d%d %d%d %d%d %d%d\n\n", cS1, cR1, cS2, cR2, cS3, cR3, cS4, cR4, cS5, cR5);*/


    /* START OF INNER LOOP */
    int cardCounter[4417] = {0};

    for (s1 =  1; s1 <= 4; s1++){
        for (r1 = 1; r1 <= 13; r1++){
            cardMatrix[s1][r1] = 1;
            for (s2 = s1; s2 <= 4; s2++){
                if (s1 == s2) {setr2 = r1 + 1;} else {setr2 = 1;}
                for (r2 = setr2; r2 <= 13; r2++){
                    cardMatrix[s2][r2] = 1;
                    for (s3 = s2; s3 <= 4; s3++){
                        if (s2 == s3) {setr3 = r2 + 1;} else {setr3 = 1;}
                        for (r3 = setr3; r3 <= 13; r3++){
                            cardMatrix[s3][r3] = 1;
                            //if (s1 != cS1 && r1 != cR1 && s2 != cS1 && r2 != cR1) {cardMatrix[s3][r3] = 0;   s3 = cS1; r3 = cR1;   cardMatrix[s3][r3] = 1;} /* GUARANTEED-RELEVANCE: 5cards */
                            //if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] == 0) { goto r3Skip;} /* IRRELEVANT: 5cards */
                            if (cardMatrix[ cS1 ][ cR1 ] == 0) { goto r3Skip;} /* IRRELEVANT: 5cards */
                            for (s4 = s3; s4 <= 4; s4++){
                                if (s3 == s4) {setr4 = r3 + 1;} else {setr4 = 1;}
                                for (r4 = setr4; r4 <= 13; r4++){
                                    cardMatrix[s4][r4] = 1;
                                    //if (s3 == cS1 && r3 == cR1) {cardMatrix[s4][r4] = 0;   s4 = cS2; r4 = cR2;   cardMatrix[s4][r4] = 1;} /* GUARANTEED-RELEVANCE: 5cards */
                                    //if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] <= 1) { goto r4Skip;} /* IRRELEVANT: 5cards */
                                    if (cardMatrix[ cS2 ][ cR2 ] == 0) { goto r4Skip;} /* IRRELEVANT: 5cards */
                                    for (s5 = s4; s5 <= 4; s5++){
                                        if (s4 == s5) {setr5 = r4 + 1;} else {setr5 = 1;}
                                        for (r5 = setr5; r5 <= 13; r5++){
                                            cardMatrix[s5][r5] = 1;
                                            //if (s4 == cS2 && r4 == cR2) {cardMatrix[s5][r5] = 0;   s5 = cS3; r5 = cR3;   cardMatrix[s5][r5] = 1;} /* GUARANTEED-RELEVANCE: 5cards */
                                            //if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] <= 2) { goto r5Skip;} /* IRRELEVANT: 5cards */
                                            if (cardMatrix[ cS3 ][ cR3 ] == 0) { goto r5Skip;} /* IRRELEVANT: 5cards */
                                            for (s6 = s5; s6 <= 4; s6++){
                                                if (s5 == s6) {setr6 = r5 + 1;} else {setr6 = 1;}
                                                for (r6 = setr6; r6 <= 13; r6++){
                                                    cardMatrix[s6][r6] = 1;
                                                    //if (s5 == cS3 && r5 == cR3) {cardMatrix[s6][r6] = 0;   s6 = cS4; r6 = cR4;   cardMatrix[s6][r6] = 1;} /* GUARANTEED-RELEVANCE: 5cards */
                                                    //if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] <= 3) { goto r6Skip;} /* IRRELEVANT: 5cards */
                                                    if (cardMatrix[ cS4 ][ cR4 ] == 0) { goto r6Skip;} /* IRRELEVANT: 5cards */
                                                    for (s7 = s6; s7 <= 4; s7++){
                                                        if (s6 == s7) {setr7 = r6 + 1;} else {setr7 = 1;}
                                                        for (r7 = setr7; r7 <= 13; r7++){
                                                            cardMatrix[s7][r7] = 1;
                                                            //if (s6 == cS4 && r6 == cR4) {cardMatrix[s7][r7] = 0;   s7 = cS5; r7 = cR5;   cardMatrix[s7][r7] = 1;} /* GUARANTEED-RELEVANCE: 5cards */
                                                            //if (cardMatrix[ cS1 ][ cR1 ] == 0 || cardMatrix[ cS2 ][ cR2 ] == 0 || cardMatrix[ cS3 ][ cR3 ] == 0 || cardMatrix[ cS4 ][ cR4 ] == 0 || cardMatrix[ cS5 ][ cR5 ] == 0) { goto r7Skip;} /* IRRELEVANT: 5cards */
                                                            if (cardMatrix[ cS5 ][ cR5 ] == 0) { goto r7Skip;} /* IRRELEVANT: 5cards */

                                                            Cards cards(s1, r1, s2, r2, s3, r3, s4, r4, s5, r5, s6, r6, s7, r7);
                                                            handRanking = handValueCalculation_NEW(cards);
                                                            if (handRanking != 4417)
                                                                cardCounter[handRanking] += 1;

                                                            counterTotal++;

                                                            if (counterTotal == 1081) /* INNER-MAX-CONDITION: 5cards*/
                                                                goto nextCombo;

                                                            r7Skip:; cardMatrix[s7][r7] = 0;
                                                        }
                                                    }
                                                    r6Skip:; cardMatrix[s6][r6] = 0;
                                                }
                                            }
                                            r5Skip:; cardMatrix[s5][r5] = 0;
                                        }
                                    }
                                    r4Skip:; cardMatrix[s4][r4] = 0;
                                }
                            }
                            r3Skip:; cardMatrix[s3][r3] = 0;
                        }
                    }
                    cardMatrix[s2][r2] = 0;
                }
            }
            cardMatrix[s1][r1] = 0;
        }
    }

    nextCombo:;

    //}}}}}}}}break;}}
    //printf("\a");

    for (int i = 0; i < 4417; i++)
        xHandRanks[i] = cardCounter [i];
}
void calculateAllHands_5_OLD_IRR_UPDATED(int xHandRanks[4417], int cS1, int cR1, int cS2, int cR2, int cS3, int cR3, int cS4, int cR4, int cS5, int cR5)
{
    // With extra break at the end // For the sake of speeding up testing, I added a break statement at the end of the cR1 loop so that I only test the first rank of each suit
    // None of these check hand values for the sake of testing purely the speed of the inner loop

    // IRRELEVANT checks are about 2-3% faster with "goto" than "continue"
    // INNER-MAX-CONDITION is about 25% faster than MAX-CONDITION, but using both is slighter slower than INNER-MAX-CONDITIONS alone.
    // I suspect the benefit of using INNER-MAX-CONDITION over MAX-CONDITION is greater for larger hands, and it was about 1% slower when I tested this on calculateAllHands_2()

    // Feb 24
    // 74 - use New IRRELEVANT CONDITION        only
    // 314 - use Old IRRELEVANT CONDITION       only
    // 1.77 - use GUARANTEED-RELEVANCE CONDITION    only
    // 2.94 - use GUARANTEED-RELEVANCE CONDITION & New IRRELEVANT CONDITION
    //  - use GUARANTEED-RELEVANCE CONDITION & Old IRRELEVANT CONDITION

    int handRanking;

    int cardMatrix[5][14] = {0};
    int suitsAndRanks[10] = {cS1, cR1, cS2, cR2, cS3, cR3, cS4, cR4, cS5, cR5};

    int counterTotal = 0;
    int setr2, setr3, setr4, setr5, setr6, setr7;
    int s1, s2, s3, s4, s5, s6, s7;
    int r1, r2, r3, r4, r5, r6, r7;
    int const FIRST_3_CARDS_THAT_MATCHED_HAND = 0;
    int const FIRST_4_CARDS_THAT_MATCHED_HAND = 1;
    int const FIRST_5_CARDS_THAT_MATCHED_HAND = 2;
    int const FIRST_6_CARDS_THAT_MATCHED_HAND = 3;


    // START OF INNER LOOP
    int cardCounter[4417] = {0};

    for (s1 =  1; s1 <= 4; s1++)
    {
        for (r1 = 1; r1 <= 13; r1++)
        {
            cardMatrix[s1][r1] = 1;
            for (s2 = s1; s2 <= 4; s2++)
            {
                setr2 = setNextRank(s1, s2, r1);
                for (r2 = setr2; r2 <= 13; r2++)
                {
                    cardMatrix[s2][r2] = 1;
                    for (s3 = s2; s3 <= 4; s3++)
                    {
                        setr3 = setNextRank(s2, s3, r2);
                        for (r3 = setr3; r3 <= 13; r3++)
                        {
                            cardMatrix[s3][r3] = 1;
                            //if (s1 != cS1 && r1 != cR1 && s2 != cS1 && r2 != cR1) {cardMatrix[s3][r3] = 0;   s3 = cS1; r3 = cR1;   cardMatrix[s3][r3] = 1;} /* GUARANTEED-RELEVANCE: 5cards */
                            if (irrelevantOld(cardMatrix, suitsAndRanks, FIRST_3_CARDS_THAT_MATCHED_HAND)) { goto r3Skip;} /* IRRELEVANT: 5cards */
                            //if (cardMatrix[ cS1 ][ cR1 ] == 0) { goto r3Skip;} /* IRRELEVANT: 5cards */
                            for (s4 = s3; s4 <= 4; s4++)
                            {
                                setr4 = setNextRank(s3, s4, r3);
                                for (r4 = setr4; r4 <= 13; r4++)
                                {
                                    cardMatrix[s4][r4] = 1;
                                    //if (s3 == cS1 && r3 == cR1) {cardMatrix[s4][r4] = 0;   s4 = cS2; r4 = cR2;   cardMatrix[s4][r4] = 1;} /* GUARANTEED-RELEVANCE: 5cards */
                                    if (irrelevantOld(cardMatrix, suitsAndRanks, FIRST_4_CARDS_THAT_MATCHED_HAND)) { goto r4Skip;} /* IRRELEVANT: 5cards */
                                    //if (cardMatrix[ cS2 ][ cR2 ] == 0) { goto r4Skip;} /* IRRELEVANT: 5cards */
                                    for (s5 = s4; s5 <= 4; s5++)
                                    {
                                        setr5 = setNextRank(s4, s5, r4);
                                        for (r5 = setr5; r5 <= 13; r5++)
                                        {
                                            cardMatrix[s5][r5] = 1;
                                            //if (s4 == cS2 && r4 == cR2) {cardMatrix[s5][r5] = 0;   s5 = cS3; r5 = cR3;   cardMatrix[s5][r5] = 1;} /* GUARANTEED-RELEVANCE: 5cards */
                                            if (irrelevantOld(cardMatrix, suitsAndRanks, FIRST_5_CARDS_THAT_MATCHED_HAND)) { goto r5Skip;} // IRRELEVANT: 5cards
                                            //if (cardMatrix[ cS3 ][ cR3 ] == 0) { goto r5Skip;} /* IRRELEVANT: 5cards */
                                            for (s6 = s5; s6 <= 4; s6++)
                                            {
                                                setr6 = setNextRank(s5, s6, r5);
                                                for (r6 = setr6; r6 <= 13; r6++)
                                                {
                                                    cardMatrix[s6][r6] = 1;
                                                    //if (s5 == cS3 && r5 == cR3) {cardMatrix[s6][r6] = 0;   s6 = cS4; r6 = cR4;   cardMatrix[s6][r6] = 1;} /* GUARANTEED-RELEVANCE: 5cards */
                                                    if (irrelevantOld(cardMatrix, suitsAndRanks, FIRST_6_CARDS_THAT_MATCHED_HAND)) { goto r6Skip;} // IRRELEVANT: 5cards
                                                    //if (cardMatrix[ cS4 ][ cR4 ] == 0) { goto r6Skip;} /* IRRELEVANT: 5cards */
                                                    for (s7 = s6; s7 <= 4; s7++)
                                                    {
                                                        setr7 = setNextRank(s6, s7, r6);
                                                        for (r7 = setr7; r7 <= 13; r7++)
                                                        {
                                                            cardMatrix[s7][r7] = 1;
                                                            //if (s6 == cS4 && r6 == cR4) {cardMatrix[s7][r7] = 0;   s7 = cS5; r7 = cR5;   cardMatrix[s7][r7] = 1;} /* GUARANTEED-RELEVANCE: 5cards */
                                                            if (cardMatrix[ cS1 ][ cR1 ] == 0 || cardMatrix[ cS2 ][ cR2 ] == 0 || cardMatrix[ cS3 ][ cR3 ] == 0 || cardMatrix[ cS4 ][ cR4 ] == 0 || cardMatrix[ cS5 ][ cR5 ] == 0) { goto r7Skip;} /* IRRELEVANT: 5cards */
                                                            //if (cardMatrix[ cS5 ][ cR5 ] == 0) { goto r7Skip;} /* IRRELEVANT: 5cards */

                                                            Cards cards(s1, r1, s2, r2, s3, r3, s4, r4, s5, r5, s6, r6, s7, r7);
                                                            handRanking = handValueCalculation_NEW(cards);
                                                            if (handRanking != 4417)
                                                                cardCounter[handRanking] += 1;

                                                            counterTotal++;

                                                            if (counterTotal == 1081) /* INNER-MAX-CONDITION: 5cards*/
                                                                goto nextCombo;

                                                            r7Skip:; cardMatrix[s7][r7] = 0;
                                                        }
                                                    }
                                                    r6Skip:; cardMatrix[s6][r6] = 0;
                                                }
                                            }
                                            r5Skip:; cardMatrix[s5][r5] = 0;
                                        }
                                    }
                                    r4Skip:; cardMatrix[s4][r4] = 0;
                                }
                            }
                            r3Skip:; cardMatrix[s3][r3] = 0;
                        }
                    }
                    cardMatrix[s2][r2] = 0;
                }
            }
            cardMatrix[s1][r1] = 0;
        }
    }

    nextCombo:;

    //printf("\a");

    for (int i = 0; i < 4417; i++)
        xHandRanks[i] = cardCounter [i];
}
// Test out this older way of calculating the inner loop to see if it's actually faster to NOT break it up into smaller methods
void calculateAllHands_5_OLD_IRR(int xHandRanks[4417], int cS1, int cR1, int cS2, int cR2, int cS3, int cR3, int cS4, int cR4, int cS5, int cR5)
{
    int handRanking;

    int cardMatrix[5][14] = {0};
    int counterTotal = 0;
    int setr2, setr3, setr4, setr5, setr6, setr7, setcR2, setcR3, setcR4, setcR5;
    int s1, s2, s3, s4, s5, s6, s7;
    int r1, r2, r3, r4, r5, r6, r7;
    int cardCounter[4417] = {0};

    for (s1 =  1; s1 <= 4; s1++)
    {
        for (r1 = 1; r1 <= 13; r1++)
        {
            cardMatrix[s1][r1] = 1;
            for (s2 = s1; s2 <= 4; s2++)
            {
                if (s1 == s2) {setr2 = r1 + 1;} else {setr2 = 1;}
                for (r2 = setr2; r2 <= 13; r2++)
                {
                    cardMatrix[s2][r2] = 1;
                    for (s3 = s2; s3 <= 4; s3++)
                    {
                        if (s2 == s3) {setr3 = r2 + 1;} else {setr3 = 1;}
                        for (r3 = setr3; r3 <= 13; r3++)
                        {
                            cardMatrix[s3][r3] = 1;
                            //if (s1 != cS1 && r1 != cR1 && s2 != cS1 && r2 != cR1) {cardMatrix[s3][r3] = 0;   s3 = cS1; r3 = cR1;   cardMatrix[s3][r3] = 1;}
                            if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] == 0) { goto r3Skip;}
                            //if (cardMatrix[ cS1 ][ cR1 ] == 0) { goto r3Skip;}
                            for (s4 = s3; s4 <= 4; s4++)
                            {
                                if (s3 == s4) {setr4 = r3 + 1;} else {setr4 = 1;}
                                for (r4 = setr4; r4 <= 13; r4++)
                                {
                                    cardMatrix[s4][r4] = 1;
                                    //if (s3 == cS1 && r3 == cR1) {cardMatrix[s4][r4] = 0;   s4 = cS2; r4 = cR2;   cardMatrix[s4][r4] = 1;}
                                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] <= 1) { goto r4Skip;}
                                    //if (cardMatrix[ cS2 ][ cR2 ] == 0) { goto r4Skip;}
                                    for (s5 = s4; s5 <= 4; s5++)
                                    {
                                        if (s4 == s5) {setr5 = r4 + 1;} else {setr5 = 1;}
                                        for (r5 = setr5; r5 <= 13; r5++)
                                        {
                                            cardMatrix[s5][r5] = 1;
                                            //if (s4 == cS2 && r4 == cR2) {cardMatrix[s5][r5] = 0;   s5 = cS3; r5 = cR3;   cardMatrix[s5][r5] = 1;}
                                            if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] <= 2) { goto r5Skip;}
                                            //if (cardMatrix[ cS3 ][ cR3 ] == 0) { goto r5Skip;} /
                                            for (s6 = s5; s6 <= 4; s6++)
                                            {
                                                if (s5 == s6) {setr6 = r5 + 1;} else {setr6 = 1;}
                                                for (r6 = setr6; r6 <= 13; r6++)
                                                {
                                                    cardMatrix[s6][r6] = 1;
                                                    //if (s5 == cS3 && r5 == cR3) {cardMatrix[s6][r6] = 0;   s6 = cS4; r6 = cR4;   cardMatrix[s6][r6] = 1;}
                                                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] <= 3) { goto r6Skip;}
                                                    //if (cardMatrix[ cS4 ][ cR4 ] == 0) { goto r6Skip;}
                                                    for (s7 = s6; s7 <= 4; s7++)
                                                    {
                                                        if (s6 == s7) {setr7 = r6 + 1;} else {setr7 = 1;}
                                                        for (r7 = setr7; r7 <= 13; r7++)
                                                        {
                                                            cardMatrix[s7][r7] = 1;
                                                            //if (s6 == cS4 && r6 == cR4) {cardMatrix[s7][r7] = 0;   s7 = cS5; r7 = cR5;   cardMatrix[s7][r7] = 1;}
                                                            if (cardMatrix[ cS1 ][ cR1 ] == 0 || cardMatrix[ cS2 ][ cR2 ] == 0 || cardMatrix[ cS3 ][ cR3 ] == 0 || cardMatrix[ cS4 ][ cR4 ] == 0 || cardMatrix[ cS5 ][ cR5 ] == 0) { goto r7Skip;}
                                                            //if (cardMatrix[ cS5 ][ cR5 ] == 0) { goto r7Skip;}

                                                            Cards cards(s1, r1, s2, r2, s3, r3, s4, r4, s5, r5, s6, r6, s7, r7);
                                                            handRanking = handValueCalculation_NEW(cards);
                                                            if (handRanking != 4417)
                                                                cardCounter[handRanking] += 1;

                                                            counterTotal++;

                                                            if (counterTotal == 1081)
                                                                goto nextCombo;

                                                            r7Skip:; cardMatrix[s7][r7] = 0;
                                                        }
                                                    }
                                                    r6Skip:; cardMatrix[s6][r6] = 0;
                                                }
                                            }
                                            r5Skip:; cardMatrix[s5][r5] = 0;
                                        }
                                    }
                                    r4Skip:; cardMatrix[s4][r4] = 0;
                                }
                            }
                            r3Skip:; cardMatrix[s3][r3] = 0;
                        }
                    }
                    cardMatrix[s2][r2] = 0;
                }
            }
            cardMatrix[s1][r1] = 0;
        }
    }

    nextCombo:;

    //printf("\a");

    for (int i = 0; i < 4417; i++)
        xHandRanks[i] = cardCounter [i];
}
/* Used to calculate every 4 card hand to build the 4cards database */
void calculateAllHands_4()
{
    int handRanking;

    int counterRF, counterSF, counter4K, counterFH, counterF, counterS, counter3K, counter2P, counterP, counterHC;
    int cardMatrix[5][14] = {0};
    int setr2, setr3, setr4, setr5, setr6, setr7, setcR2, setcR3, setcR4;


    //2cards
    for (int cS1 = 1; cS1 <=  4; cS1++){
        for (int cR1 = 1; cR1 <= 13; cR1++){
            for (int cS2 = cS1; cS2 <= 4; cS2++){
                if (cS1 == cS2) {setcR2 = cR1 + 1;} else {setcR2 = 1;}
                for (int cR2 = setcR2; cR2 <= 13; cR2++){
                    //3cards
                    for (int cS3 = cS2; cS3 <= 4; cS3++){
                        if (cS2 == cS3) {setcR3 = cR2 + 1;} else {setcR3 = 1;}
                        for (int cR3 = setcR3; cR3 <= 13; cR3++){
                            //4cards
                            for (int cS4 = cS3; cS4 <= 4; cS4++){
                                if (cS3 == cS4) {setcR4 = cR3 + 1;} else {setcR4 = 1;}
                                for (int cR4 = setcR4; cR4 <= 13; cR4++){


                                    /* START OF INNER LOOP */
                                    int counterTotal = counterRF = counterSF = counter4K = counterFH = counterF = counterS = counter3K = counter2P = counterP = counterHC = 0;
                                    int cardCounter[4417] = {0};

                                    for (int s1 =  1; s1 <= 4; s1++){
                                        for (int r1 = 1; r1 <= 13; r1++){
                                            cardMatrix[s1][r1] = 1;
                                            for (int s2 = s1; s2 <= 4; s2++){
                                                if (s1 == s2) {setr2 = r1 + 1;} else {setr2 = 1;}
                                                for (int r2 = setr2; r2 <= 13; r2++){
                                                    cardMatrix[s2][r2] = 1;
                                                    for (int s3 = s2; s3 <= 4; s3++){
                                                        if (s2 == s3) {setr3 = r2 + 1;} else {setr3 = 1;}
                                                        for (int r3 = setr3; r3 <= 13; r3++){
                                                            cardMatrix[s3][r3] = 1;
                                                            for (int s4 = s3; s4 <= 4; s4++){
                                                                if (s3 == s4) {setr4 = r3 + 1;} else {setr4 = 1;}
                                                                for (int r4 = setr4; r4 <= 13; r4++){
                                                                    cardMatrix[s4][r4] = 1;
                                                                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] == 0) { goto r4Skip;} /* IRRELEVANT: 4cards */
                                                                    for (int s5 = s4; s5 <= 4; s5++){
                                                                        if (s4 == s5) {setr5 = r4 + 1;} else {setr5 = 1;}
                                                                        for (int r5 = setr5; r5 <= 13; r5++){
                                                                            cardMatrix[s5][r5] = 1;
                                                                            if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] <= 1) { goto r5Skip;} /* IRRELEVANT: 4cards */
                                                                            for (int s6 = s5; s6 <= 4; s6++){
                                                                                if (s5 == s6) {setr6 = r5 + 1;} else {setr6 = 1;}
                                                                                for (int r6 = setr6; r6 <= 13; r6++){
                                                                                    cardMatrix[s6][r6] = 1;
                                                                                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] <= 2) { goto r6Skip;} /* IRRELEVANT: 4cards */
                                                                                    for (int s7 = s6; s7 <= 4; s7++){
                                                                                        if (s6 == s7) {setr7 = r6 + 1;} else {setr7 = 1;}
                                                                                        for (int r7 = setr7; r7 <= 13; r7++){
                                                                                            cardMatrix[s7][r7] = 1;
                                                                                            if (cardMatrix[ cS1 ][ cR1 ] == 0 || cardMatrix[ cS2 ][ cR2 ] == 0 || cardMatrix[ cS3 ][ cR3 ] == 0 || cardMatrix[ cS4 ][ cR4 ] == 0) { goto r7Skip;} /* IRRELEVANT: 4cards */

                                                                                            Cards cards(s1, r1, s2, r2, s3, r3, s4, r4, s5, r5, s6, r6, s7, r7);
                                                                                            handRanking = handValueCalculation(cards);
                                                                                            if (handRanking != 4417)
                                                                                                cardCounter[handRanking] += 1;

                                                                                            counterTotal++;

                                                                                            if (counterTotal == 17296) /* INNER-MAX-CONDITION: 4cards*/
                                                                                                goto nextCombo;

                                                                                            r7Skip:; cardMatrix[s7][r7] = 0;}}
                                                                                    r6Skip:; cardMatrix[s6][r6] = 0;}}
                                                                            r5Skip:; cardMatrix[s5][r5] = 0;}}
                                                                    r4Skip:; cardMatrix[s4][r4] = 0;}}
                                                            cardMatrix[s3][r3] = 0;}}
                                                    cardMatrix[s2][r2] = 0;}}
                                            cardMatrix[s1][r1] = 0;}}

                                    nextCombo:;
                                }}}}}}}}
}
/* Used to calculate every 3 card hand to build the 3cards database */
void calculateAllHands_3()
{
    // With extra break at the end
    // ????s full
    // 99s without checking hand values - use MAX-CONDITION
    // 97s without checking hand values - use MAX-CONDITION 2nd check
    // 98s without checking hand values - use INNER-MAX-CONDITION
    // 99s without checking hand values - use INNER-MAX-CONDITION 2nd check
    // 98s without checking hand values - use both

    int handRanking;

    int counterRF, counterSF, counter4K, counterFH, counterF, counterS, counter3K, counter2P, counterP, counterHC;
    int cardMatrix[5][14] = {0};
    int setr2, setr3, setr4, setr5, setr6, setr7, setcR2, setcR3;


    //2cards
    for (int cS1 = 1; cS1 <=  4; cS1++){
        for (int cR1 = 1; cR1 <= 13; cR1++){                   printf("%d %d \n\n", cS1, cR1);
            for (int cS2 = cS1; cS2 <= 4; cS2++){
                if (cS1 == cS2) {setcR2 = cR1 + 1;} else {setcR2 = 1;}
                for (int cR2 = setcR2; cR2 <= 13; cR2++){
                    //3cards
                    for (int cS3 = cS2; cS3 <= 4; cS3++){
                        if (cS2 == cS3) {setcR3 = cR2 + 1;} else {setcR3 = 1;}
                        for (int cR3 = setcR3; cR3 <= 13; cR3++){


                            /* START OF INNER LOOP */
                            int counterTotal = counterRF = counterSF = counter4K = counterFH = counterF = counterS = counter3K = counter2P = counterP = counterHC = 0;
                            int cardCounter[4417] = {0};

                            for (int s1 =  1; s1 <= 4; s1++){
                                for (int r1 = 1; r1 <= 13; r1++){
                                    cardMatrix[s1][r1] = 1;
                                    for (int s2 = s1; s2 <= 4; s2++){
                                        if (s1 == s2) {setr2 = r1 + 1;} else {setr2 = 1;}
                                        for (int r2 = setr2; r2 <= 13; r2++){
                                            cardMatrix[s2][r2] = 1;
                                            for (int s3 = s2; s3 <= 4; s3++){
                                                if (s2 == s3) {setr3 = r2 + 1;} else {setr3 = 1;}
                                                for (int r3 = setr3; r3 <= 13; r3++){
                                                    cardMatrix[s3][r3] = 1;
                                                    for (int s4 = s3; s4 <= 4; s4++){
                                                        if (s3 == s4) {setr4 = r3 + 1;} else {setr4 = 1;}
                                                        for (int r4 = setr4; r4 <= 13; r4++){
                                                            cardMatrix[s4][r4] = 1;
                                                            for (int s5 = s4; s5 <= 4; s5++){
                                                                if (s4 == s5) {setr5 = r4 + 1;} else {setr5 = 1;}
                                                                for (int r5 = setr5; r5 <= 13; r5++){
                                                                    cardMatrix[s5][r5] = 1;
                                                                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] == 0) { goto r5Skip;} /* IRRELEVANT: 3cards */
                                                                    for (int s6 = s5; s6 <= 4; s6++){
                                                                        if (s5 == s6) {setr6 = r5 + 1;} else {setr6 = 1;}
                                                                        for (int r6 = setr6; r6 <= 13; r6++){
                                                                            cardMatrix[s6][r6] = 1;
                                                                            if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] <= 1) { goto r6Skip;} /* IRRELEVANT: 3cards */
                                                                            for (int s7 = s6; s7 <= 4; s7++){
                                                                                if (s6 == s7) {setr7 = r6 + 1;} else {setr7 = 1;}
                                                                                for (int r7 = setr7; r7 <= 13; r7++){
                                                                                    cardMatrix[s7][r7] = 1;
                                                                                    if (cardMatrix[ cS1 ][ cR1 ] == 0 || cardMatrix[ cS2 ][ cR2 ] == 0 || cardMatrix[ cS3 ][ cR3 ] == 0) { goto r7Skip;} /* IRRELEVANT: 3cards */

                                                                                    Cards cards(s1, r1, s2, r2, s3, r3, s4, r4, s5, r5, s6, r6, s7, r7);
                                                                                    handRanking = handValueCalculation(cards);
                                                                                    if (handRanking != 4417)
                                                                                        cardCounter[handRanking] += 1;

                                                                                    counterTotal++;

                                                                                    if (counterTotal == 211876) /* INNER-MAX-CONDITION: 3cards*/
                                                                                        goto nextCombo;

                                                                                    r7Skip:; cardMatrix[s7][r7] = 0;}}
                                                                            r6Skip:; cardMatrix[s6][r6] = 0;}}
                                                                    r5Skip:; cardMatrix[s5][r5] = 0;}}
                                                            cardMatrix[s4][r4] = 0;}}
                                                    cardMatrix[s3][r3] = 0;}}
                                            cardMatrix[s2][r2] = 0;}}
                                    cardMatrix[s1][r1] = 0;}}

                            nextCombo:;
                        }}}}}}
}
/* Used to calculate every 2 card hand to build the 2cards database */
void calculateAllHands_2()
{
    // No extra break at the end
    // 1669s full
    // 274s without checking hand values - use MAX-CONDITION
    // 275s without checking hand values - use MAX-CONDITION 2nd check
    // 270s without checking hand values - use MAX-CONDITION 3rd check
    // 269s without checking hand values - use MAX-CONDITION 4th check
    // 276s without checking hand values - use INNER-MAX-CONDITION
    // 277s without checking hand values - use INNER-MAX-CONDITION 2nd check
    // 274s without checking hand values - use INNER-MAX-CONDITION 3rd check
    // 279s without checking hand values - use both

    // 269s without checking hand values - use INNER-MAX-CONDITION between s6 & r6
    // 268s without checking hand values - use INNER-MAX-CONDITION between r6 & s7
    // 270s without checking hand values - use INNER-MAX-CONDITION between r6 & s7 2nd check
    // 269s without checking hand values - use INNER-MAX-CONDITION between s7 & r7

    int handRanking;

    int counterRF, counterSF, counter4K, counterFH, counterF, counterS, counter3K, counter2P, counterP, counterHC;
    int cardMatrix[5][14] = {0};
    int setr2, setr3, setr4, setr5, setr6, setr7, setcR2;


    //2cards
    for (int cS1 = 1; cS1 <=  4; cS1++){
        for (int cR1 = 1; cR1 <= 13; cR1++){                   printf("%d %d \n\n", cS1, cR1);
            for (int cS2 = cS1; cS2 <= 4; cS2++){
                if (cS1 == cS2) {setcR2 = cR1 + 1;} else {setcR2 = 1;}
                for (int cR2 = setcR2; cR2 <= 13; cR2++){


                    /* START OF INNER LOOP */
                    int counterTotal = counterRF = counterSF = counter4K = counterFH = counterF = counterS = counter3K = counter2P = counterP = counterHC = 0;
                    int cardCounter[4417] = {0};

                    for (int s1 =  1; s1 <= 4; s1++){
                        for (int r1 = 1; r1 <= 13; r1++){
                            // if (s1 > cS1 || (s1 == cS1 && r1 > cR1)) {goto nextCombo;} /* MAX-CONDITION: 2/3/4/5/6cards - Earlier conditions like this are needed for when the last of cS#cR# is 413 */
                            cardMatrix[s1][r1] = 1;
                            for (int s2 = s1; s2 <= 4; s2++){
                                if (s1 == s2) {setr2 = r1 + 1;} else {setr2 = 1;}
                                for (int r2 = setr2; r2 <= 13; r2++){
                                    //if (s1 == cS1 && r1 == cR1 && (s2 > cS2 || (s2 == cS2 && r2 > cR2))) {goto nextCombo;} /* MAX-CONDITION: 2/3/4/5/6cards */
                                    cardMatrix[s2][r2] = 1;
                                    for (int s3 = s2; s3 <= 4; s3++){
                                        if (s2 == s3) {setr3 = r2 + 1;} else {setr3 = 1;}
                                        for (int r3 = setr3; r3 <= 13; r3++){
                                            cardMatrix[s3][r3] = 1;
                                            for (int s4 = s3; s4 <= 4; s4++){
                                                if (s3 == s4) {setr4 = r3 + 1;} else {setr4 = 1;}
                                                for (int r4 = setr4; r4 <= 13; r4++){
                                                    cardMatrix[s4][r4] = 1;
                                                    for (int s5 = s4; s5 <= 4; s5++){
                                                        if (s4 == s5) {setr5 = r4 + 1;} else {setr5 = 1;}
                                                        for (int r5 = setr5; r5 <= 13; r5++){
                                                            cardMatrix[s5][r5] = 1;
                                                            for (int s6 = s5; s6 <= 4; s6++){
                                                                if (s5 == s6) {setr6 = r5 + 1;} else {setr6 = 1;}
                                                                for (int r6 = setr6; r6 <= 13; r6++){
                                                                    cardMatrix[s6][r6] = 1;
                                                                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] == 0) { goto r6Skip;} /* IRRELEVANT: 2cards */
                                                                    for (int s7 = s6; s7 <= 4; s7++){
                                                                        if (s6 == s7) {setr7 = r6 + 1;} else {setr7 = 1;}
                                                                        for (int r7 = setr7; r7 <= 13; r7++){
                                                                            cardMatrix[s7][r7] = 1;
                                                                            if (cardMatrix[ cS1 ][ cR1 ] == 0 || cardMatrix[ cS2 ][ cR2 ] == 0) { goto r7Skip;} /* IRRELEVANT: 2cards */

                                                                            Cards cards(s1, r1, s2, r2, s3, r3, s4, r4, s5, r5, s6, r6, s7, r7);
                                                                            handRanking = handValueCalculation(cards);
                                                                            if (handRanking != 4417)
                                                                                cardCounter[handRanking] += 1;

                                                                            counterTotal++;

                                                                            if (counterTotal == 2118760) /* INNER-MAX-CONDITION: 2cards*/
                                                                                goto nextCombo;


                                                                            r7Skip:; cardMatrix[s7][r7] = 0;}}
                                                                    r6Skip:; cardMatrix[s6][r6] = 0;}}
                                                            cardMatrix[s5][r5] = 0;}}
                                                    cardMatrix[s4][r4] = 0;}}
                                            cardMatrix[s3][r3] = 0;}}
                                    cardMatrix[s2][r2] = 0;}}
                            cardMatrix[s1][r1] = 0;}}

                    nextCombo:;
                }}}}

    cout<<"\a";
}


/* Used to calculate the strength-graph of a particular 6 card hand */
void calculateOneHand_6(int cards[6], int xHandRanks[4417])
{
    int handRanking;

    int counterRF, counterSF, counter4K, counterFH, counterF, counterS, counter3K, counter2P, counterP, counterHC;
    int cardMatrix[5][14] = {0};
    int setr2, setr3, setr4, setr5, setr6, setr7, setcR2, setcR3, setcR4, setcR5, setcR6;

    sort(cards, cards + 6);
    int cS1 = cards[0]/100;
    int cR1 = cards[0]%100;
    int cS2 = cards[1]/100;
    int cR2 = cards[1]%100;
    int cS3 = cards[2]/100;
    int cR3 = cards[2]%100;
    int cS4 = cards[3]/100;
    int cR4 = cards[3]%100;
    int cS5 = cards[4]/100;
    int cR5 = cards[4]%100;
    int cS6 = cards[5]/100;
    int cR6 = cards[5]%100;

    /* START OF INNER LOOP */
    int counterTotal = counterRF = counterSF = counter4K = counterFH = counterF = counterS = counter3K = counter2P = counterP = counterHC = 0;
    int cardCounter[4417] = {0};

    for (int s1 =  1; s1 <= 4; s1++){
        for (int r1 = 1; r1 <= 13; r1++){
            cardMatrix[s1][r1] = 1;
            for (int s2 = s1; s2 <= 4; s2++){
                if (s1 == s2) {setr2 = r1 + 1;} else {setr2 = 1;}
                for (int r2 = setr2; r2 <= 13; r2++){
                    cardMatrix[s2][r2] = 1;
                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] + cardMatrix[ cS6 ][ cR6 ] == 0) { goto r2Skip;} /* EARLY TT1: 6cards */
                    for (int s3 = s2; s3 <= 4; s3++){
                        if (s2 == s3) {setr3 = r2 + 1;} else {setr3 = 1;}
                        for (int r3 = setr3; r3 <= 13; r3++){
                            cardMatrix[s3][r3] = 1;
                            if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] + cardMatrix[ cS6 ][ cR6 ] <= 1) { goto r3Skip;} /* EARLY TT1: 6cards */
                            for (int s4 = s3; s4 <= 4; s4++){
                                if (s3 == s4) {setr4 = r3 + 1;} else {setr4 = 1;}
                                for (int r4 = setr4; r4 <= 13; r4++){
                                    cardMatrix[s4][r4] = 1;
                                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] + cardMatrix[ cS6 ][ cR6 ] <= 2) { goto r4Skip;} /* EARLY TT1: 6cards */
                                    for (int s5 = s4; s5 <= 4; s5++){
                                        if (s4 == s5) {setr5 = r4 + 1;} else {setr5 = 1;}
                                        for (int r5 = setr5; r5 <= 13; r5++){
                                            cardMatrix[s5][r5] = 1;
                                            if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] + cardMatrix[ cS6 ][ cR6 ] <= 3) { goto r5Skip;} /* EARLY TT1: 6cards */
                                            for (int s6 = s5; s6 <= 4; s6++){
                                                if (s5 == s6) {setr6 = r5 + 1;} else {setr6 = 1;}
                                                for (int r6 = setr6; r6 <= 13; r6++){
                                                    cardMatrix[s6][r6] = 1;
                                                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] + cardMatrix[ cS6 ][ cR6 ] <= 4) { goto r6Skip;} /* EARLY TT1: 6cards */
                                                    for (int s7 = s6; s7 <= 4; s7++){
                                                        if (s6 == s7) {setr7 = r6 + 1;} else {setr7 = 1;}
                                                        for (int r7 = setr7; r7 <= 13; r7++){
                                                            cardMatrix[s7][r7] = 1;
                                                            if (cardMatrix[ cS1 ][ cR1 ] == 0 || cardMatrix[ cS2 ][ cR2 ] == 0 || cardMatrix[ cS3 ][ cR3 ] == 0 || cardMatrix[ cS4 ][ cR4 ] == 0 || cardMatrix[ cS5 ][ cR5 ] == 0 || cardMatrix[ cS6 ][ cR6 ] == 0) { goto r7Skip;} /* EARLY TT1: 6cards */

                                                            Cards cards(s1, r1, s2, r2, s3, r3, s4, r4, s5, r5, s6, r6, s7, r7);
                                                            handRanking = handValueCalculation(cards);
                                                            if (handRanking != 4417)
                                                                cardCounter[handRanking] += 1;

                                                            counterTotal++;

                                                            if (counterTotal == 46) /* INNER-MAX-CONDITION: 6cards*/
                                                                goto nextCombo;

                                                            r7Skip:; cardMatrix[s7][r7] = 0;}}
                                                    r6Skip:; cardMatrix[s6][r6] = 0;}}
                                            r5Skip:; cardMatrix[s5][r5] = 0;}}
                                    r4Skip:; cardMatrix[s4][r4] = 0;}}
                            r3Skip:; cardMatrix[s3][r3] = 0;}}
                    r2Skip:; cardMatrix[s2][r2] = 0;}}
            cardMatrix[s1][r1] = 0;}}

    nextCombo:;
    for (int a = 0; a <= 4416; a++)
        xHandRanks[a] = cardCounter[a];
}
/* Used to calculate the strength-graph of a particular 5 card hand */
void calculateOneHand_5(int cards[5], int xHandRanks[4417])
{
    int handRanking;

    int counterRF, counterSF, counter4K, counterFH, counterF, counterS, counter3K, counter2P, counterP, counterHC;
    int cardMatrix[5][14] = {0};
    int setr2, setr3, setr4, setr5, setr6, setr7, setcR2, setcR3, setcR4, setcR5;

    sort(cards, cards + 5);
    int cS1 = cards[0]/100;
    int cR1 = cards[0]%100;
    int cS2 = cards[1]/100;
    int cR2 = cards[1]%100;
    int cS3 = cards[2]/100;
    int cR3 = cards[2]%100;
    int cS4 = cards[3]/100;
    int cR4 = cards[3]%100;
    int cS5 = cards[4]/100;
    int cR5 = cards[4]%100;

    /* START OF INNER LOOP */
    int counterTotal = counterRF = counterSF = counter4K = counterFH = counterF = counterS = counter3K = counter2P = counterP = counterHC = 0;
    int cardCounter[4417] = {0};

    for (int s1 =  1; s1 <= 4; s1++){
        for (int r1 = 1; r1 <= 13; r1++){
            cardMatrix[s1][r1] = 1;
            for (int s2 = s1; s2 <= 4; s2++){
                if (s1 == s2) {setr2 = r1 + 1;} else {setr2 = 1;}
                for (int r2 = setr2; r2 <= 13; r2++){
                    cardMatrix[s2][r2] = 1;
                    for (int s3 = s2; s3 <= 4; s3++){
                        if (s2 == s3) {setr3 = r2 + 1;} else {setr3 = 1;}
                        for (int r3 = setr3; r3 <= 13; r3++){
                            cardMatrix[s3][r3] = 1;
                            if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] == 0) { goto r3Skip;} /* IRRELEVANT: 5cards */
                            for (int s4 = s3; s4 <= 4; s4++){
                                if (s3 == s4) {setr4 = r3 + 1;} else {setr4 = 1;}
                                for (int r4 = setr4; r4 <= 13; r4++){
                                    cardMatrix[s4][r4] = 1;
                                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] <= 1) { goto r4Skip;} /* IRRELEVANT: 5cards */
                                    for (int s5 = s4; s5 <= 4; s5++){
                                        if (s4 == s5) {setr5 = r4 + 1;} else {setr5 = 1;}
                                        for (int r5 = setr5; r5 <= 13; r5++){
                                            cardMatrix[s5][r5] = 1;
                                            if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] <= 2) { goto r5Skip;} /* IRRELEVANT: 5cards */
                                            for (int s6 = s5; s6 <= 4; s6++){
                                                if (s5 == s6) {setr6 = r5 + 1;} else {setr6 = 1;}
                                                for (int r6 = setr6; r6 <= 13; r6++){
                                                    cardMatrix[s6][r6] = 1;
                                                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] + cardMatrix[ cS5 ][ cR5 ] <= 3) { goto r6Skip;} /* IRRELEVANT: 5cards */
                                                    for (int s7 = s6; s7 <= 4; s7++){
                                                        if (s6 == s7) {setr7 = r6 + 1;} else {setr7 = 1;}
                                                        for (int r7 = setr7; r7 <= 13; r7++){
                                                            cardMatrix[s7][r7] = 1;
                                                            if (cardMatrix[ cS1 ][ cR1 ] == 0 || cardMatrix[ cS2 ][ cR2 ] == 0 || cardMatrix[ cS3 ][ cR3 ] == 0 || cardMatrix[ cS4 ][ cR4 ] == 0 || cardMatrix[ cS5 ][ cR5 ] == 0) { goto r7Skip;} /* IRRELEVANT: 5cards */

                                                            Cards cards(s1, r1, s2, r2, s3, r3, s4, r4, s5, r5, s6, r6, s7, r7);
                                                            handRanking = handValueCalculation(cards);
                                                            if (handRanking != 4417)
                                                                cardCounter[handRanking] += 1;

                                                            if (handRanking == 1602)
                                                                printf("%d %d %d %d %d %d %d\n", s1 * 100 + r1, s2 * 100 + r2, s3 * 100 + r3, s4 * 100 + r4, s5 * 100 + r5, s6 * 100 + r6, s7 * 100 + r7);

                                                            counterTotal++;

                                                            if (counterTotal == 1081) /* INNER-MAX-CONDITION: 5cards*/
                                                                goto nextCombo;

                                                            r7Skip:; cardMatrix[s7][r7] = 0;}}
                                                    r6Skip:; cardMatrix[s6][r6] = 0;}}
                                            r5Skip:; cardMatrix[s5][r5] = 0;}}
                                    r4Skip:; cardMatrix[s4][r4] = 0;}}
                            r3Skip:; cardMatrix[s3][r3] = 0;}}
                    cardMatrix[s2][r2] = 0;}}
            cardMatrix[s1][r1] = 0;}}

    nextCombo:;
    for (int a = 0; a <= 4416; a++)
        xHandRanks[a] = cardCounter[a];
    //for (int i = 0; i <= 4416; i++)
    //    printf("%d, ", xHandRanks[i]);
}
/* Used to calculate the strength-graph of a particular 4 card hand */
void calculateOneHand_4(int cards[4], int xHandRanks[4417])
{
    int handRanking;

    int counterRF, counterSF, counter4K, counterFH, counterF, counterS, counter3K, counter2P, counterP, counterHC;
    int cardMatrix[5][14] = {0};
    int setr2, setr3, setr4, setr5, setr6, setr7, setcR2, setcR3, setcR4;

    sort(cards, cards + 4);
    int cS1 = cards[0]/100;
    int cR1 = cards[0]%100;
    int cS2 = cards[1]/100;
    int cR2 = cards[1]%100;
    int cS3 = cards[2]/100;
    int cR3 = cards[2]%100;
    int cS4 = cards[3]/100;
    int cR4 = cards[3]%100;

    /* START OF INNER LOOP */
    int counterTotal = counterRF = counterSF = counter4K = counterFH = counterF = counterS = counter3K = counter2P = counterP = counterHC = 0;
    int cardCounter[4417] = {0};

    for (int s1 =  1; s1 <= 4; s1++){
        for (int r1 = 1; r1 <= 13; r1++){
            cardMatrix[s1][r1] = 1;
            for (int s2 = s1; s2 <= 4; s2++){
                if (s1 == s2) {setr2 = r1 + 1;} else {setr2 = 1;}
                for (int r2 = setr2; r2 <= 13; r2++){
                    cardMatrix[s2][r2] = 1;
                    for (int s3 = s2; s3 <= 4; s3++){
                        if (s2 == s3) {setr3 = r2 + 1;} else {setr3 = 1;}
                        for (int r3 = setr3; r3 <= 13; r3++){
                            cardMatrix[s3][r3] = 1;
                            for (int s4 = s3; s4 <= 4; s4++){
                                if (s3 == s4) {setr4 = r3 + 1;} else {setr4 = 1;}
                                for (int r4 = setr4; r4 <= 13; r4++){
                                    cardMatrix[s4][r4] = 1;
                                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] == 0) { goto r4Skip;} /* IRRELEVANT: 4cards */
                                    for (int s5 = s4; s5 <= 4; s5++){
                                        if (s4 == s5) {setr5 = r4 + 1;} else {setr5 = 1;}
                                        for (int r5 = setr5; r5 <= 13; r5++){
                                            cardMatrix[s5][r5] = 1;
                                            if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] <= 1) { goto r5Skip;} /* IRRELEVANT: 4cards */
                                            for (int s6 = s5; s6 <= 4; s6++){
                                                if (s5 == s6) {setr6 = r5 + 1;} else {setr6 = 1;}
                                                for (int r6 = setr6; r6 <= 13; r6++){
                                                    cardMatrix[s6][r6] = 1;
                                                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] + cardMatrix[ cS4 ][ cR4 ] <= 2) { goto r6Skip;} /* IRRELEVANT: 4cards */
                                                    for (int s7 = s6; s7 <= 4; s7++){
                                                        if (s6 == s7) {setr7 = r6 + 1;} else {setr7 = 1;}
                                                        for (int r7 = setr7; r7 <= 13; r7++){
                                                            cardMatrix[s7][r7] = 1;
                                                            if (cardMatrix[ cS1 ][ cR1 ] == 0 || cardMatrix[ cS2 ][ cR2 ] == 0 || cardMatrix[ cS3 ][ cR3 ] == 0 || cardMatrix[ cS4 ][ cR4 ] == 0) { goto r7Skip;} /* IRRELEVANT: 4cards */

                                                            Cards cards(s1, r1, s2, r2, s3, r3, s4, r4, s5, r5, s6, r6, s7, r7);
                                                            handRanking = handValueCalculation(cards);
                                                            if (handRanking != 4417)
                                                                cardCounter[handRanking] += 1;

                                                            counterTotal++;

                                                            if (counterTotal == 17296) /* INNER-MAX-CONDITION: 4cards*/
                                                                goto nextCombo;

                                                            r7Skip:; cardMatrix[s7][r7] = 0;}}
                                                    r6Skip:; cardMatrix[s6][r6] = 0;}}
                                            r5Skip:; cardMatrix[s5][r5] = 0;}}
                                    r4Skip:; cardMatrix[s4][r4] = 0;}}
                            cardMatrix[s3][r3] = 0;}}
                    cardMatrix[s2][r2] = 0;}}
            cardMatrix[s1][r1] = 0;}}

    nextCombo:;
    for (int a = 0; a <= 4416; a++)
        xHandRanks[a] = cardCounter[a];
}
/* Used to calculate the strength-graph of a particular 3 card hand */
void calculateOneHand_3(int cards[3], int xHandRanks[4417])
{
    int handRanking;

    int counterRF, counterSF, counter4K, counterFH, counterF, counterS, counter3K, counter2P, counterP, counterHC;
    int cardMatrix[5][14] = {0};
    int setr2, setr3, setr4, setr5, setr6, setr7, setcR2, setcR3;

    sort(cards, cards + 3);
    int cS1 = cards[0]/100;
    int cR1 = cards[0]%100;
    int cS2 = cards[1]/100;
    int cR2 = cards[1]%100;
    int cS3 = cards[2]/100;
    int cR3 = cards[2]%100;

    /* START OF INNER LOOP */
    int counterTotal = counterRF = counterSF = counter4K = counterFH = counterF = counterS = counter3K = counter2P = counterP = counterHC = 0;
    int cardCounter[4417] = {0};

    for (int s1 =  1; s1 <= 4; s1++){
        for (int r1 = 1; r1 <= 13; r1++){
            cardMatrix[s1][r1] = 1;
            for (int s2 = s1; s2 <= 4; s2++){
                if (s1 == s2) {setr2 = r1 + 1;} else {setr2 = 1;}
                for (int r2 = setr2; r2 <= 13; r2++){
                    cardMatrix[s2][r2] = 1;
                    for (int s3 = s2; s3 <= 4; s3++){
                        if (s2 == s3) {setr3 = r2 + 1;} else {setr3 = 1;}
                        for (int r3 = setr3; r3 <= 13; r3++){
                            cardMatrix[s3][r3] = 1;
                            for (int s4 = s3; s4 <= 4; s4++){
                                if (s3 == s4) {setr4 = r3 + 1;} else {setr4 = 1;}
                                for (int r4 = setr4; r4 <= 13; r4++){
                                    cardMatrix[s4][r4] = 1;
                                    for (int s5 = s4; s5 <= 4; s5++){
                                        if (s4 == s5) {setr5 = r4 + 1;} else {setr5 = 1;}
                                        for (int r5 = setr5; r5 <= 13; r5++){
                                            cardMatrix[s5][r5] = 1;
                                            if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] == 0) { goto r5Skip;} /* IRRELEVANT: 3cards */
                                            for (int s6 = s5; s6 <= 4; s6++){
                                                if (s5 == s6) {setr6 = r5 + 1;} else {setr6 = 1;}
                                                for (int r6 = setr6; r6 <= 13; r6++){
                                                    cardMatrix[s6][r6] = 1;
                                                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] + cardMatrix[ cS3 ][ cR3 ] <= 1) { goto r6Skip;} /* IRRELEVANT: 3cards */
                                                    for (int s7 = s6; s7 <= 4; s7++){
                                                        if (s6 == s7) {setr7 = r6 + 1;} else {setr7 = 1;}
                                                        for (int r7 = setr7; r7 <= 13; r7++){
                                                            cardMatrix[s7][r7] = 1;
                                                            if (cardMatrix[ cS1 ][ cR1 ] == 0 || cardMatrix[ cS2 ][ cR2 ] == 0 || cardMatrix[ cS3 ][ cR3 ] == 0) { goto r7Skip;} /* IRRELEVANT: 3cards */

                                                            Cards cards(s1, r1, s2, r2, s3, r3, s4, r4, s5, r5, s6, r6, s7, r7);
                                                            handRanking = handValueCalculation(cards);
                                                            if (handRanking != 4417)
                                                                cardCounter[handRanking] += 1;

                                                            counterTotal++;

                                                            if (counterTotal == 211876) /* INNER-MAX-CONDITION: 3cards*/
                                                                goto nextCombo;

                                                            r7Skip:; cardMatrix[s7][r7] = 0;}}
                                                    r6Skip:; cardMatrix[s6][r6] = 0;}}
                                            r5Skip:; cardMatrix[s5][r5] = 0;}}
                                    cardMatrix[s4][r4] = 0;}}
                            cardMatrix[s3][r3] = 0;}}
                    cardMatrix[s2][r2] = 0;}}
            cardMatrix[s1][r1] = 0;}}

    nextCombo:;
    for (int a = 0; a <= 4416; a++)
        xHandRanks[a] = cardCounter[a];
}
/* Used to calculate the strength-graph of a particular 2 card hand */
void calculateOneHand_2(int cards[2], int xHandRanks[4417])
{
    int handRanking;

    int counterRF, counterSF, counter4K, counterFH, counterF, counterS, counter3K, counter2P, counterP, counterHC;
    int cardMatrix[5][14] = {0};
    int setr2, setr3, setr4, setr5, setr6, setr7, setcR2;

    sort(cards, cards + 2);
    int cS1 = cards[0]/100;
    int cR1 = cards[0]%100;
    int cS2 = cards[1]/100;
    int cR2 = cards[1]%100;

    /* START OF INNER LOOP */
    int counterTotal = counterRF = counterSF = counter4K = counterFH = counterF = counterS = counter3K = counter2P = counterP = counterHC = 0;
    int cardCounter[4417] = {0};

    for (int s1 =  1; s1 <= 4; s1++){
        for (int r1 = 1; r1 <= 13; r1++){
            cardMatrix[s1][r1] = 1;
            for (int s2 = s1; s2 <= 4; s2++){
                if (s1 == s2) {setr2 = r1 + 1;} else {setr2 = 1;}
                for (int r2 = setr2; r2 <= 13; r2++){
                    cardMatrix[s2][r2] = 1;
                    for (int s3 = s2; s3 <= 4; s3++){
                        if (s2 == s3) {setr3 = r2 + 1;} else {setr3 = 1;}
                        for (int r3 = setr3; r3 <= 13; r3++){
                            cardMatrix[s3][r3] = 1;
                            for (int s4 = s3; s4 <= 4; s4++){
                                if (s3 == s4) {setr4 = r3 + 1;} else {setr4 = 1;}
                                for (int r4 = setr4; r4 <= 13; r4++){
                                    cardMatrix[s4][r4] = 1;
                                    for (int s5 = s4; s5 <= 4; s5++){
                                        if (s4 == s5) {setr5 = r4 + 1;} else {setr5 = 1;}
                                        for (int r5 = setr5; r5 <= 13; r5++){
                                            cardMatrix[s5][r5] = 1;
                                            for (int s6 = s5; s6 <= 4; s6++){
                                                if (s5 == s6) {setr6 = r5 + 1;} else {setr6 = 1;}
                                                for (int r6 = setr6; r6 <= 13; r6++){
                                                    cardMatrix[s6][r6] = 1;
                                                    if (cardMatrix[ cS1 ][ cR1 ] + cardMatrix[ cS2 ][ cR2 ] == 0) { goto r6Skip;} /* IRRELEVANT: 2cards */
                                                    for (int s7 = s6; s7 <= 4; s7++){
                                                        if (s6 == s7) {setr7 = r6 + 1;} else {setr7 = 1;}
                                                        for (int r7 = setr7; r7 <= 13; r7++){
                                                            cardMatrix[s7][r7] = 1;
                                                            if (cardMatrix[ cS1 ][ cR1 ] == 0 || cardMatrix[ cS2 ][ cR2 ] == 0) { goto r7Skip;} /* IRRELEVANT: 2cards */

                                                            Cards cards(s1, r1, s2, r2, s3, r3, s4, r4, s5, r5, s6, r6, s7, r7);
                                                            handRanking = handValueCalculation(cards);
                                                            if (handRanking != 4417)
                                                                cardCounter[handRanking] += 1;

                                                            counterTotal++;

                                                            if (counterTotal == 2118760) /* INNER-MAX-CONDITION: 2cards*/
                                                                goto nextCombo;

                                                            r7Skip:; cardMatrix[s7][r7] = 0;}}
                                                    r6Skip:; cardMatrix[s6][r6] = 0;}}
                                            cardMatrix[s5][r5] = 0;}}
                                    cardMatrix[s4][r4] = 0;}}
                            cardMatrix[s3][r3] = 0;}}
                    cardMatrix[s2][r2] = 0;}}
            cardMatrix[s1][r1] = 0;}}

    nextCombo:;
    for (int a = 0; a <= 4416; a++)
        xHandRanks[a] = cardCounter[a];
}








int main()
{
    int cardsInHand = 5;             // How many cards are in your hand (hole cards + community cards)
    bool calculateAllHands = true; // Calculate the hands for every possible N-card combination (T), or calculate the hands for a particular N-card combination (F)

    int xHandRanks1[4417] = {0};
    int xHandRanks2[4417] = {0};
    int xHandRanks3[4417] = {0};
    int setr2, setr3, setr4, setr5, setr6, setr7, setcR2, setcR3, setcR4, setcR5;

    time_t startTime = time(&startTime);
    time_t endTime;


    //2cards
    for (int cS1 = 1; cS1 <=  4; cS1++){
        for (int cR1 = 1; cR1 <= 13; cR1++){                   //printf("%d%d \n\n", cS1, cR1);
            for (int cS2 = cS1; cS2 <= 4; cS2++){
                setcR2 = setNextRank(cS1, cS2, cR1);
                for (int cR2 = setcR2; cR2 <= 13; cR2++){                   printf("%d%d %d%d\n\n", cS1, cR1, cS2, cR2);
                    //3cards
                    for (int cS3 = cS2; cS3 <= 4; cS3++){
                        setcR3 = setNextRank(cS2, cS3, cR2);
                        for (int cR3 = setcR3; cR3 <= 13; cR3++){
                            //4cards
                            for (int cS4 = cS3; cS4 <= 4; cS4++){
                                setcR4 = setNextRank(cS3, cS4, cR3);
                                for (int cR4 = setcR4; cR4 <= 13; cR4++){
                                    //5cards
                                    for (int cS5 = cS4; cS5 <= 4; cS5++){
                                        setcR5 = setNextRank(cS4, cS5, cR4);
                                        for (int cR5 = setcR5; cR5 <= 13; cR5++){

                                            //printf("%d%d %d%d %d%d %d%d %d%d\n\n", cS1, cR1, cS2, cR2, cS3, cR3, cS4, cR4, cS5, cR5);
                                            //calculateAllHands_5_GR (xHandRanks1, cS1, cR1, cS2, cR2, cS3, cR3, cS4, cR4, cS5, cR5);
                                            //calculateAllHands_5_NEW_IRR (xHandRanks2, cS1, cR1, cS2, cR2, cS3, cR3, cS4, cR4, cS5, cR5);
                                            calculateAllHands_5_OLD_IRR (xHandRanks3, cS1, cR1, cS2, cR2, cS3, cR3, cS4, cR4, cS5, cR5);
                                            //calculateAllHands_5_OLD_IRR_UPDATED (xHandRanks3, cS1, cR1, cS2, cR2, cS3, cR3, cS4, cR4, cS5, cR5);

                                            /*for (int i = 0; i < 4417; i++)
                                            {
                                                if( xHandRanks1[i] != xHandRanks2[i] )
                                                    printf("GR & NIRR   MISMATCH at %d\n", i);
                                                if( xHandRanks1[i] != xHandRanks3[i] )
                                                    printf("GR & OIRR   MISMATCH at %d\n", i);
                                                if( xHandRanks2[i] != xHandRanks3[i] )
                                                    printf("NIRR & OIRR MISMATCH at %d\n", i);
                                            }*/

                                        }}}}}}}}}}

    time(&endTime);
    cout << '\a';
    cout<< endTime - startTime << " seconds passed, press enter to end";
    cin.ignore();

    /*if (calculateAllHands == true)
    {
        if (cardsInHand == 7)        // A comprehensive test primarily meant to test the efficiency of handValueCalculation()
            calculateAllHands_7();
        else if (cardsInHand == 6)   // For 2 to 6 cards, tests the efficiency of handValueCalculation() and the MAX-CONDITION, EARLY TT1, and cardMatrix[][]-goto inner-loop optimizations
            calculateAllHands_6();
        else if (cardsInHand == 5)
            calculateAllHands_5_NEW();
        else if (cardsInHand == 4)
            calculateAllHands_4();
        else if (cardsInHand == 3)
            calculateAllHands_3();
        else if (cardsInHand == 2)
            calculateAllHands_2();
        else
            printf("Cards in hand are not from 2 to 7");
    }
    else
    {
        int xHandRanks[4417];

        if (cardsInHand == 6)
        {
            int cards[6] = {102, 305, 304, 306, 407, 409};
            calculateOneHand_6(cards, xHandRanks);
        }
        else if (cardsInHand == 5)
        {
            int cards[5] = {305, 304, 306, 407, 408};
            calculateOneHand_5(cards, xHandRanks);
        }
        else if (cardsInHand == 4)
        {
            int cards[4] = {304, 305, 306, 407};
            calculateOneHand_4(cards, xHandRanks);
        }
        else if (cardsInHand == 3)
        {
            int cards[3] = {304, 305, 306};
            calculateOneHand_3(cards, xHandRanks);
        }
        else if (cardsInHand == 2)
        {
            int cards[2] = {305, 306};
            calculateOneHand_2(cards, xHandRanks);
        }
        else
            printf("Cards in hand are not from 2 to 6");
    }*/
}
