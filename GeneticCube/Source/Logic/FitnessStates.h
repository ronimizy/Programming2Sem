//
// Created by Георгий Круглов on 12.05.2021.
//

#ifndef GENETICCUBE_FITNESSSTATES_H
#define GENETICCUBE_FITNESSSTATES_H

enum FitnessStates {
    Random = 0,
    BottomCross = 100,
    Bars = 200,
    FullSide = 300,
    Pyramid = 400,
    Bag = 500,
    TopCross = 600,
    NoTopCorners = 700,
    Solved = 1000
};

#endif //GENETICCUBE_FITNESSSTATES_H
