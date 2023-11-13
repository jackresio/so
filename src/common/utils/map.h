#ifndef SISTEMI_OPERATIVI_MAP_H
#define SISTEMI_OPERATIVI_MAP_H

struct point {
    double x;
    double y;
} typedef Point;

/**
 * Get a random position and save in position
 * @param position Location to save new random position
 */
void rand_position(Point *position);

/**
 * Generate position for port.
 * The first four position are in the corner of the map
 * @param position pointer to a position
 */
void get_port_position(Point *position);

/**
 * Compare two point
 * @param p1 first point
 * @param p2 second point
 * @return one if the points are in the seme spot, 0 otherwise
 */
int compare_position(Point p1, Point p2);

double calc_distance(Point a, Point b);


#endif
