#ifndef SNR_H
#define SNR_H

#include <vector>

/*!
 * Calculates the energy of the signal
 *
 * @param original content of the signal
 * @return energy of the signal
 */
long calcEs(std::vector<short>& original);

/*!
 * Calculates the energy of the noise
 *
 * @param original content of the original signal
 * @param noise content of the noise signal
 * @return energy of the noise
 * @throws invalid_argument if original and noise vector
 *  don't have the same size
 */
long calcEn(std::vector<short>& original, std::vector<short>& noise);

/**
 * Calculates the Signal-To-Noise ratio (SNR)
 *
 * @param Es energy signal
 * @param En energy noise
 * @return Signal-To-Noise ratio
 */
double calcSNR(long Es, long En);

#endif //SNR_H

