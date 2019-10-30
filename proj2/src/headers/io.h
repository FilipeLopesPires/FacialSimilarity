#ifndef IO_H
#define IO_H

#include <sndfile.hh>
#include <string>

/*!
 * Verifies if the file on the provided path is:
 * <ul>
 *  <li>in wav format</li>
 *  <li>in PCM_16 format</li>
 *  <li>it has the number of channels specified</li>
 * </ul>
 * @param sndFile reference to a already initialized sndFile
 * @param path just to display better error messages
 * @param channels number of channels that the file must have.
 *  If a negative number or zero is received the numbers of channels
 *  is not checked.
 */
void checkFileToRead(SndfileHandle& sndFile, const char* path, int channels=-1);

/*!
 * Verifies if the file on the provided path was opened successfully
 * @param sndFile reference to a already initialized sndFile
 * @param path just to display better error messages
 */
void checkFileOpenSuccess(SndfileHandle& sndFile, const char* path);

#endif
