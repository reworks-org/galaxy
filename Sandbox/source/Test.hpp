//
//  Test.hpp
//  Sandbox
//
//  Created by reworks on 28/10/2017.
//  Copyright (c) 2017 reworks. All rights reserved.
//

#ifndef SANDBOX_TEST_HPP_
#define SANDBOX_TEST_HPP_

#include <re/types/State.hpp>
#include <re/managers/StateManager.hpp>

class Test : public sl::State<Test>
{
public:
	~Test() override;

	void load() override;
	void unload() override;

	void event(ALLEGRO_EVENT* event) override;
	void update(double dt) override;
	void render() override;

private:
	

};

#endif