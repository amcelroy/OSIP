#ifndef INSITELASER_H
#define INSITELASER_H

#include <Peripherals/laser.hpp>

class InsiteLaser : public Laser
{
public:
    InsiteLaser();

    ~InsiteLaser();

    void run() override;

    void init() override;

    void pause() override;

    void reset() override;

    void stop() override;
};

#endif // INSITELASER_H
