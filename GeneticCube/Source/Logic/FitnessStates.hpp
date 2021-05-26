//
// Created by Георгий Круглов on 12.05.2021.
//

#ifndef GENETICCUBE_FITNESSSTATES_HPP
#define GENETICCUBE_FITNESSSTATES_HPP

//@no-format
enum FitnessStates {
    Random        =    0,
    BottomCross   =  100,
    Bars          =  200,
    FullSide      =  300,
    Pyramid       =  400,
    Bag           =  500,
    TopCross      =  600,
    NoTopCorners  =  700,
    Solved        = 1000,
};
//@format

#endif //GENETICCUBE_FITNESSSTATES_HPP
