#ifndef LASER_HPP
#define LASER_HPP

#include <Peripherals/peripheral.hpp>
#include <vector>

using namespace OSIP;
using namespace Peripherals;
using namespace std;

namespace OSIP{
    namespace Peripherals{
        class Laser : public Peripheral
        {
        private:


        public:
            Laser();

            ~Laser() override;

        };
    }
}

#endif // LASER_HPP
