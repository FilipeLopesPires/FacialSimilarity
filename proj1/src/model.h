
#ifndef MODEL_H
#define MODEL_H

#include <cmath>
#include <fstream>
#include <list>
#include <map>
#include <set>

/*!
 * Data structures to build a Markov's Model
 */
namespace model {

/*!
 * Auxiliary Structure to hold statistic information for the occurrences
 * of a context or of a letter after a given context
 */
typedef struct {
    /*!
     * Number of occurrences
     */
    int count;
    /*!
     * Probability of occurrence
     */
    double probability;
} Statistics;

/*!
 * Used as value for the statsTable member of the Model class
 */
typedef struct {
    /*!
     * Occurrence statistics of the characters of the alphabet
     *  appearing after a specific context
     */
    std::map<char, Statistics> nextCharStats;

    /*!
     * Statistics of a specific context
     */
    Statistics stats;
} ContextStatistics;

/*!
 * Markov's Model Implementation that holds the counts and probability of occurrences of a context
 *  and letters after a context. These metrics are used to then calculate
 *  the model entropy and to generate text.
 */
class Model {
   private:
    /*!
     * Context length
     */
    int ctxLen;

    /*!
     * Smoothing parameter
     */
    double alpha;

    /*!
     * Alphabet of the test data
     */
    std::set<char> abc;

    /*!
     * Sum of occurrences of all contexts
     */
    int totalContextsCount;

    /*!
     * Table with occurrence statistics of contexts and letters
     *  after a context
     * ```
     * {
     *  "context": {
     *      stats: {
     *          count: X,
     *          probability: X.X
     *      }
     *      nextCharStats: {
     *          'letter': {
     *              count: X,
     *              probability: X.X
     *          },
     *          ...
     *      }
     *  },
     *  ...
     * }
     * ```
     */
    std::map<std::string, ContextStatistics> statsTable;

    /*!
     * Model entropy
     */
    double entropy;

    /*!
     * Function called inside the parseFile() function. Is used
     *  to update the calculations of:
     *   - probabilities of occurrences of contexts
     *   - probabilities of occurrences of letters after a given context
     *   - model entropy
     */
    void calcProbabilitiesAndEntropy();

   public:
    /*!
     * Class constructor
     * @param ctxLen Context length
     * @param alpha Smoothing parameter
     */
    Model(int ctxLen, double alpha) {
        this->ctxLen = ctxLen;
        this->alpha = alpha;
    }

    /*!
     * Getter of the alphabet member
     * @return model alphabet
     */
    auto& getABC() const { return abc; }

    /*!
     * Getter of the table with occurrence statistics member
     * @return table with occurrence statistics
     */
    auto& getStatsTable() const { return statsTable; }

    /**
     * Getter of the model entropy
     * @return the model entropy
     */
    auto getModelEntropy() { return entropy; };

    /**
     * Parse a list of files to train the model by updating
     *  the occurrences counts for contexts and letters after a given context
     * @param input list of already opened files, ready to read
     */
    void parseFile(std::list<std::fstream*>& input);
};

}

#endif