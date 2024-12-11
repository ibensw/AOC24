#include "readfile.h"
#include <fmt/format.h>
#include <list>

struct FileMap {
    struct Entry {
        int offset;
        int id;
        int size;
        int padding;
    };
    std::list<Entry> disk; // Allows for insertion and does not invalidate iterators when inserting/removing elements

    FileMap(const std::vector<char> &data)
    {
        int offset = 0;
        int id = 0;
        for (; 2 * id < data.size() - 2; ++id) {
            auto size = data[2 * id] - '0';
            auto padding = data[2 * id + 1] - '0';
            disk.emplace_back(offset, id, size, padding);
            offset += size + padding;
        }
        auto size = data[2 * id] - '0';
        disk.emplace_back(offset, id, size, 0);
    }

    void print() const
    {
        for (const auto &entry : disk) {
            for (int i = 0; i < entry.size; ++i) {
                fmt::print("{}", entry.id);
            }
            if (entry.padding > 0) {
                fmt::print("{:.<{}}", ".", entry.padding);
            }
        }
        fmt::print("\n");
    }

    unsigned int size() const
    {
        return disk.back().offset + disk.back().size;
    }

    std::list<Entry>::iterator findFirstFree(std::list<Entry>::iterator begin)
    {
        for (auto it = begin; it != disk.end(); ++it) {
            if (it->padding) {
                return it;
            }
        }
        return disk.end();
    }

    void compact()
    {
        auto firstFree = findFirstFree(disk.begin());
        auto lastBlock = --disk.end();
        while (firstFree != disk.end() && lastBlock != disk.begin()) {
            auto &current = *firstFree;
            auto moveSize = std::min(firstFree->padding, lastBlock->size);
            auto insertPosition = firstFree;
            insertPosition++;
            auto newEntry = disk.emplace(insertPosition, current.offset + current.size, lastBlock->id, moveSize,
                                         current.padding - moveSize);
            current.padding = 0;
            if (moveSize == lastBlock->size) {
                disk.pop_back();
                lastBlock = --disk.end();
                lastBlock->padding = 0;
            } else {
                lastBlock->size -= moveSize;
            }
            firstFree = findFirstFree(newEntry);
        }
    }

    using CheckSumType = unsigned long long;

    CheckSumType checksum() const
    {
        CheckSumType sum = 0;
        int count = 0;
        for (const auto &entry : disk) {
            auto backup = sum;
            count++;
            CheckSumType partial = (entry.offset + entry.offset + entry.size - 1); // This won't overflow
            partial *= entry.size;
            partial /= 2;
            partial *= entry.id;
            sum += partial;
            if (backup > sum) {
                fmt::print("Entry {} / {}\n", count, disk.size());
                fmt::print("offset={}, id={}, size={}, padding={}\n", entry.offset, entry.id, entry.size,
                           entry.padding);
                fmt::print("Partial sum: {}\n", partial);
                throw std::runtime_error("Overflow detected");
            }
        }
        return sum;
    }
};

int main(int argc, char *argv[])
{
    auto input = readFile(argv[1]);
    FileMap fileMap(input);
    // fileMap.print();
    fmt::print("Size: {}, Entries: {}\n", fileMap.size(), fileMap.disk.size());
    fileMap.compact();
    fmt::print("Size: {}, Entries: {}\n", fileMap.size(), fileMap.disk.size());
    // fileMap.print();
    fmt::print("Checksum: {}\n", fileMap.checksum());
    return 0;
}
