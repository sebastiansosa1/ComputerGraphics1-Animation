//  ========================================================================
//  COSC363: Computer Graphics (2022);  University of Canterbury.
//
//  RailModels.h
//  Header file for a collection of models for a railway scene
//  ========================================================================


void floor(GLUquadricObj* q);

void tracks(float, float);

void tracksTies (void);

void tunnel(float, float, int, int, float);

void station(void);

void controlTower(void);

void engine(float, float, float, float);

void wagon(float, float, float, float);

void sun(GLUquadricObj*);