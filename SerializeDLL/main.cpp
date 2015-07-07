/**
 * g++ test_boost_serialization.cpp -lboost_serialization -std=c++0x
 *
 * **/

#include <fstream>
#include <iostream>
#include <string>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>


class private_members
{
    double *data;
    size_t count;

    public:
    private_members()
    {
        data = NULL;
        count = 0;
    }

    private_members(double *d, int c)
    {
        data = d;
        count = c;
    }

    private_members(int seed, int c)
    {
        count = c;

        data = new double[count];

        for (size_t i = 0; i < count; i++)
        {
            data[i] = i+seed;
        }

    }

    double data_at(int index) { return data[index]; }

    void print_data()
    {
        for (size_t i = 0; i < count; i++)
        {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }

    // Note the explicit work done.
    template<class Archive>
    void save(Archive &ar, const unsigned int version) const
    {
        ar & count;

        if (count != -1)
        {
            for (size_t i = 0; i < count; i++)
            {
                ar & data[i];
            }
        }
    }

    template<class Archive>
    void load(Archive &ar, const unsigned int version)
    {
        int c;
        ar & c;

        if (c == -1)
        {
            count = -1;
            return;
        }

        if (count != c)
        {
            free(data);
            data = NULL;
            data = new double[count];
            count = c;
        }
            

        for (size_t i = 0; i < c; i++)
        {
            ar & data[i];
        }

    }

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        boost::serialization::split_member(ar, *this, version);
    }

    inline int return_count() { return count; }
};

int main(int argc, char **argv)
{

    // Serialization stuff.
    std::ofstream ofs;
    std::ifstream ifs;

    // Struct with Primitive Pointer Version.
    private_members pr_temp(0, 10), pr_restore(1, 20);

    // Save.
    std::cout << "Printing Saved Data: ";
    pr_temp.print_data();
    std::cout << std::endl;

    ofs.open("test_struct.ar");
    boost::archive::text_oarchive oa(ofs);
    oa << pr_temp;
    ofs.close();

    // Restore.
    std::cout << "Printing Existing Data: ";
    pr_restore.print_data();

    ifs.open("test_struct.ar");
    boost::archive::text_iarchive ia(ifs);
    ia >> pr_restore;
    ifs.close();

    std::cout << "Printing Restored Data: ";
    pr_restore.print_data();



    // Vector version.
    std::vector<private_members> pr_m, pr_r;

    for (size_t i = 0; i < 10; i++)
    {
        private_members pr_temp(i, 10);
        pr_m.push_back(pr_temp);
    }

    // Save.
    std::cout << "Saving data count: " << pr_m.size() << std::endl;

    ofs.open("test_vector.ar");
    boost::archive::text_oarchive oav(ofs);
    oav << pr_m;
    ofs.close();

    // Restore.
    std::cout << "Existing data count: " << pr_r.size() << std::endl;

    ifs.open("test_vector.ar");
    boost::archive::text_iarchive iav(ifs);
    iav >> pr_r;
    ifs.close();

    std::cout << "Restored data count: " << pr_r.size() << std::endl;

    return 0;
}