/*
 * SPDX-FileCopyrightText: 2006-2021 Istituto Italiano di Tecnologia (IIT)
 * SPDX-FileCopyrightText: 2006-2010 RobotCub Consortium
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <yarp/os/Time.h>
#include <yarp/os/NetType.h>
#include <string>

#include <catch.hpp>
#include <harness.h>

using namespace yarp::os;

void test_delay(double target, double limit)
{
    double t1 = Time::now();
    Time::delay(target);
    double t2 = Time::now();
    double dt = t2 - t1 - target;
    bool inLimits = (-limit < dt) && (dt < limit);
    std::string s = std::string("tested delay of ") + std::to_string(target) +
        std::string("s, delay was late(+) or early(-) by ") +
        std::to_string((double)(dt * 1000)) +
        std::string(" ms");
    INFO(s);
    printf("%s\n", s.c_str());
    CHECK(inLimits);
}

TEST_CASE("os::TimeTest", "[yarp::os]")
{
    SECTION("testing delay (there will be a short pause)...")
    {
        test_delay ( 3.000, 0.010);
        test_delay ( 0.300, 0.010);
        test_delay ( 0.030, 0.010);
    }
}
