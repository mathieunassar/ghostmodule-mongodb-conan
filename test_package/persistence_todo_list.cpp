/*
 * Copyright 2020 Mathieu Nassar
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <bsoncxx/exception/exception.hpp>
#include <ghost/module/StdoutLogger.hpp>
#include <ghost/module/Module.hpp>
#include <ghost/module/ModuleBuilder.hpp>
#include <ghost/persistence/DataManager.hpp>
#include <ghost/persistence_extension/PersistenceExtension.hpp>
#include <ghost/persistence_extension/PersistenceExtensionBuilder.hpp>
#include <ghost/persistence_mongodb/DatabaseMongoDB.hpp>
#include <ghost/persistence_mongodb/PersistenceMongoDBExtension.hpp>
#include <ghost/persistence_mongodb/PersistenceMongoDBExtensionBuilder.hpp>

#include "persistency_todo_list.pb.h"

/***************************
	TRY IT: Run this program and add or remove your own TODOs.
***************************/

class TodoListModule
{
public:
	bool initialize(const ghost::Module& module)
	{
		return true;
	}

	// The execution of the module reads the parameters provided to the program call.
	// It can list, add or remove parameters.
	bool run(const ghost::Module& module)
	{
		return false;
	}
};

int main(int argc, char** argv)
{
	TodoListModule myModule;

	// Configuration of the module. We provide here all the components to the builder.
	auto builder = ghost::ModuleBuilder::create();
	// This line will provide the intialization method.
	builder->setInitializeBehavior(std::bind(&TodoListModule::initialize, &myModule, std::placeholders::_1));
	// This line will provide the run method, which will be called cyclically.
	builder->setRunningBehavior(std::bind(&TodoListModule::run, &myModule, std::placeholders::_1));
	builder->setLogger(ghost::StdoutLogger::create());
	// Parse the program options to determine what to do:
	builder->setProgramOptions(argc, argv);

	// Add the Persistence extension in order to use a shared ghost::DataManager
	builder->addExtensionBuilder(ghost::PersistenceExtensionBuilder::create());

	// We will use the new MongoDB extension as a database
	auto mongoBuilder = ghost::PersistenceMongoDBExtensionBuilder::create();
	mongoBuilder->addConnection({"mongodb://localhost:27017"});
	builder->addExtensionBuilder(mongoBuilder);

	// The following line creates the module with all the parameters, and names it "myModuleInstance0".
	std::shared_ptr<ghost::Module> module = builder->build();

	// If the build process is successful, we can start the module.
	// If it were not successful, we would have nullptr here.
	if (module) module->start();

	// Start blocks until the module ends.
	std::cout << "test package completed" << std::endl;
	return 0;
}
