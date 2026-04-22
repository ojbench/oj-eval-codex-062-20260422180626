#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP

#include <fstream>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 2>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);

    // Ensure the file exists; if not, initialise it
    void ensure_file_exists() {
        // Try to open for input to check existence
        file.open(file_name, std::ios::in | std::ios::binary);
        if (!file) {
            // Create and initialise header space
            file.clear();
            file.open(file_name, std::ios::out | std::ios::binary);
            int tmp = 0;
            for (int i = 0; i < info_len; ++i) file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        }
        if (file.is_open()) file.close();
    }

public:
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        file.open(file_name, std::ios::out | std::ios::binary);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    // 读出第n个int的值赋给tmp，1_base
    void get_info(int &tmp, int n) {
        if (n > info_len || n <= 0) return;
        ensure_file_exists();
        file.open(file_name, std::ios::in | std::ios::binary);
        if (!file) return;
        file.seekg(static_cast<std::streamoff>((n - 1) * sizeof(int)), std::ios::beg);
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    // 将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n) {
        if (n > info_len || n <= 0) return;
        ensure_file_exists();
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) return;
        file.seekp(static_cast<std::streamoff>((n - 1) * sizeof(int)), std::ios::beg);
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    // 在文件合适位置写入类对象t，并返回写入的位置索引index
    int write(T &t) {
        ensure_file_exists();
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) return -1;
        file.seekp(0, std::ios::end);
        std::streamoff index = file.tellp();
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
        return static_cast<int>(index);
    }

    // 用t的值更新位置索引index对应的对象
    void update(T &t, const int index) {
        ensure_file_exists();
        file.open(file_name, std::ios::in | std::ios::out | std::ios::binary);
        if (!file) return;
        file.seekp(static_cast<std::streamoff>(index), std::ios::beg);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }

    // 读出位置索引index对应的T对象
    void read(T &t, const int index) {
        ensure_file_exists();
        file.open(file_name, std::ios::in | std::ios::binary);
        if (!file) return;
        file.seekg(static_cast<std::streamoff>(index), std::ios::beg);
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }

    // 删除位置索引index对应的对象（无空间回收，忽略实现）
    void Delete(int /*index*/) {
        // No-op for the easy version without reclamation
    }
};


#endif //BPT_MEMORYRIVER_HPP

