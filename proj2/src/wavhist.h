#ifndef WAVHIST_H
#define WAVHIST_H

#include <iostream>
#include <map>
#include <sndfile.hh>
#include <vector>

// class WAVHist {
//    private:
//     std::vector<std::map<short, size_t>> counts;

//    public:
//     WAVHist(const SndfileHandle& sfh) { counts.resize(sfh.channels()); }

//     void update(const std::vector<short>& samples) {
//         size_t n{};
//         for (auto s : samples) counts[n++ % counts.size()][s]++;
//     }

//     void dump(const size_t channel) const {
//         for (auto [value, counter] : counts[channel])
//             std::cout << value << '\t' << counter << '\n';
//     }
// };

class WAVHist {
   private:
    std::map<short, size_t> counts;
    std::vector<short> data;
    size_t n{0};
    int num{};
    short avg{};

   public:
    WAVHist(const SndfileHandle& sfh) { num = sfh.channels(); }

    void update(const std::vector<short>& samples) {
        for (auto s : samples) {
            if (n++ % num == 0) {
                counts[avg / num]++;
                avg = 0;
            }
            avg += s;
        }
    }

    void dump(const size_t channel) const {
        for (auto [value, counter] : counts)
            std::cout << value << '\t' << counter << '\n';
    }
};

#endif
