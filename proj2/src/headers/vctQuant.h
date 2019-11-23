#ifndef SNR_H
#define SNR_H

#include <sndfile.hh>
#include <vector>

/*!
 * Calculates the energy of the signal
 *
 * @param signal content of the signal
 * @return energy of the signal
 */
double calcEs(std::vector<short>& signal);

/*!
 * Calculates the energy of the noise
 *
 * @param original content of the original signal
 * @param noise content of the noise signal
 * @return energy of the noise
 * @throws invalid_argument if original and noise vector
 *  don't have the same size
 */
double calcEn(std::vector<short>& original, std::vector<short>& noise);

/*!
 * Calculates the maximum absolute error
 *
 * @param original content of the original signal
 * @param noise content of the noise signal
 * @return energy of the noise
 * @throws invalid_argument if original and noise vector
 *  don't have the same size
 */
double calcEnMax(std::vector<short>& original, std::vector<short>& noise);

/*!
 * Calculates the Signal-To-Noise ratio (SNR)
 *
 * @param Es energy signal
 * @param En energy noise
 * @return Signal-To-Noise ratio
 */
double calcSNR(double Es, double En);

/*!
 * Applies a sliding window to a wav SndFile to
 *  retrieve several blocks of the same size
 *  with some overlap among them
 *
 * @param blocks where the blocks read will be stored
 * @param sndFile from which the blocks will be retrieved
 * @param blockSize size of each block. The sliding window will
 *  have the same size
 * @param overlapFactor between adjacent blocks. This also tells
 *  how much the sliding window advances to read the next block
 */
void retrieveBlocks(std::vector<std::vector<short>>& blocks,
                    SndfileHandle& sndFile, int blockSize, float overlapFactor);

double calculateError(std::vector<std::vector<short>>& blocks,
                      std::vector<std::vector<short>>& codeBook);

#endif
