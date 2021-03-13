#include "Scene2.h"
#include "Cars.h"
#include "Obstacles.h"

int start2 = 0;

Scene2::Scene2(GLFWwindow* inWind)
{
	window = inWind;
}

void Scene2::Start()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	rampTex = Texture2D::LoadFromFile("images/tinyRamp.png");

	sceneBuff = m_Registry.create();
	cocoBuff = m_Registry.create();
	bloomBuff = m_Registry.create();
	blurBuff = m_Registry.create();
	m_Registry.emplace<PostEffect>(sceneBuff);
	m_Registry.emplace<CubeCoCoEffect>(cocoBuff);
	m_Registry.emplace<CombinedBloom>(bloomBuff);
	m_Registry.emplace<Blur>(blurBuff);
	m_Registry.get<PostEffect>(sceneBuff).Init(width, height);
	m_Registry.get<CubeCoCoEffect>(cocoBuff).Init(width, height);
	m_Registry.get<CombinedBloom>(bloomBuff).Init(width, height);
	m_Registry.get<Blur>(blurBuff).Init(width, height);



	cubes.push_back(LUT3D("cubes/Neutral-512.cube"));
	cubes.push_back(LUT3D("cubes/Cool.cube"));
	cubes.push_back(LUT3D("cubes/Warm.cube"));
	cubes.push_back(LUT3D("cubes/Darken-512.cube"));

	AudioEngine& engine = AudioEngine::Instance();


	//play event
	AudioEvent& oldMusic = engine.GetEvent("Menu Music");
	oldMusic.Stop();

	AudioEvent& newMusic = engine.CreateEventW("Ambient", "{18c986e1-88b0-45ce-82c7-567d3447f2e8}");
	newMusic.Play();

	AudioEvent& slipstream = engine.CreateEventW("Slipstream", "{50d08bc6-b9f1-4411-906f-69506bd36f13}");
	AudioEvent& drift = engine.CreateEventW("Drift", "{3eb39553-5d08-456c-998b-822942c1f860}");
	AudioEvent& multiNitro = engine.CreateEventW("MultiNitro", "{6d8f789b-95db-4007-bd66-f26c1f377b3c}");

	slipstream.StopImmediately();
	drift.StopImmediately();
	multiNitro.StopImmediately();

	camera = Camera::Create();
	std::string fileName = "monkey.obj";
	entt::entity testModel = m_Registry.create();
	entt::entity shader = m_Registry.create();
	m_PCar = m_Registry.create();
	m_enemy = m_Registry.create();
	m_Obstacle = m_Registry.create();
	entt::entity Track = m_Registry.create();
	m_Hazard = m_Registry.create();
	m_Gearbox = m_Registry.create();
	m_GearboxLever = m_Registry.create();
	m_Accelerometer = m_Registry.create();
	m_Needle = m_Registry.create();
	/*m_Particles1 = m_Registry.create();
	m_Particles2 = m_Registry.create();*/
	//cards
	m_Card = m_Registry.create();
	m_TransparentBlack = m_Registry.create();
	m_PauseMenu = m_Registry.create();


	/*entt::entity morphTest = m_Registry.create();
	m_Registry.emplace<syre::MorphRenderer>(morphTest);
	m_Registry.emplace<syre::Texture>(morphTest, "images/Car2.png");
	m_Registry.emplace<syre::Transform>(morphTest, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.f, 0.0f, 0.0f), glm::vec3(1.0f));
	m_Registry.get<syre::MorphRenderer>(morphTest).AddFrame("morph01.obj");
	m_Registry.get<syre::MorphRenderer>(morphTest).AddFrame("morph02.obj");
	*/

	//track and scenery
	m_Registry.emplace<syre::Mesh>(Track, "objects/Track2.obj");
	m_Registry.emplace<syre::Transform>(Track, glm::vec3(8.0f, 8.0f, -0.5f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(2.2f));
	m_Registry.emplace<syre::Texture>(Track, "images/PossibleRoad.png");

	m_Registry.emplace<syre::Mesh>(m_Hazard, "objects/RoadHazard.obj");
	m_Registry.emplace<syre::Transform>(m_Hazard, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_Hazard, "images/Apex.png");

	m_Registry.emplace<syre::Mesh>(m_Gearbox, "objects/Gearbox.obj");
	m_Registry.emplace<syre::Transform>(m_Gearbox, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_Gearbox, "images/GearBoxNeutral.png");

	m_Registry.emplace<syre::Mesh>(m_GearboxLever, "objects/Lever.obj");
	m_Registry.emplace<syre::Transform>(m_GearboxLever, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_GearboxLever, "images/GearboxLever.png");

	m_Registry.emplace<syre::Mesh>(m_Accelerometer, "objects/Accelerometer.obj");
	m_Registry.emplace<syre::Transform>(m_Accelerometer, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_Accelerometer, "images/Accelerometer1.png");

	m_Registry.emplace<syre::MorphRenderer>(m_Needle);
	m_Registry.emplace<syre::Transform>(m_Needle, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_Needle, "images/Finish.png");
	m_Registry.get<syre::MorphRenderer>(m_Needle).AddFrame("objects/NeedleLeft.obj");
	m_Registry.get<syre::MorphRenderer>(m_Needle).AddFrame("objects/Needle.obj");
	m_Registry.get<syre::MorphRenderer>(m_Needle).AddFrame("objects/NeedleRight.obj");

	m_Registry.emplace<syre::Mesh>(m_TransparentBlack, "objects/RoadHazard.obj");
	m_Registry.emplace<syre::Transform>(m_TransparentBlack, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_TransparentBlack, "images/TransparentBlack.png");

	m_Registry.emplace<syre::Mesh>(m_PauseMenu, "objects/RoadHazard.obj");
	m_Registry.emplace<syre::Transform>(m_PauseMenu, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.25f));
	m_Registry.emplace<syre::Texture>(m_PauseMenu, "images/PauseMenu.png");



	entt::entity start = m_Registry.create();
	m_Registry.emplace<syre::MorphRenderer>(start);
	m_Registry.emplace<syre::Transform>(start, glm::vec3(15.0f, -50.0f, 0.0f), glm::vec3(90.0f, 0.0f, 190.0f), glm::vec3(1.0f));
	m_Registry.emplace<syre::Texture>(start, "images/Start.png");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/start2.obj");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/start2.obj");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/START2.obj");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/START3.obj");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/START4.obj");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/START5.obj");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/START6.obj");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/START7.obj");
	m_Registry.get<syre::MorphRenderer>(start).AddFrame("objects/START8.obj");


	entt::entity finish = m_Registry.create();
	m_Registry.emplace<syre::MorphRenderer>(finish);
	m_Registry.emplace<syre::Transform>(finish, glm::vec3(-270.0f, -135.0f, 0.0f), glm::vec3(90.0f, 0.0f, 150.0f), glm::vec3(1.0f));
	m_Registry.emplace<syre::Texture>(finish, "images/Finish.png");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH1.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH1.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH2.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH3.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH4.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH5.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH6.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH7.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH8.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH9.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH10.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH11.obj");
	m_Registry.get<syre::MorphRenderer>(finish).AddFrame("objects/FINISH12.obj");


	/*

	//trees
	entt::entity trackTrees = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(trackTrees, "objects/TreesMap.obj");
	m_Registry.emplace<syre::Transform>(trackTrees, glm::vec3(-30.0f, 125.0f, -0.5f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(2.2f));
	m_Registry.emplace<syre::Texture>(trackTrees, "images/Tree.png");
	*/
	//terrain
	entt::entity trackTerrain = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(trackTerrain, "objects/TerrainMap.obj");
	m_Registry.emplace<syre::Transform>(trackTerrain, glm::vec3(-30.0f, 125.0f, -0.5f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(2.2f));
	m_Registry.emplace<syre::Texture>(trackTerrain, "images/Terrain.png");
	/*
	//signs
	entt::entity trackSigns = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(trackSigns, "objects/SignsMap.obj");
	m_Registry.emplace<syre::Transform>(trackSigns, glm::vec3(-30.0f, 125.0f, -0.5f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(2.2f));
	m_Registry.emplace<syre::Texture>(trackSigns, "images/Signs.png");

	//sharp rocks
	entt::entity trackSharpRocks = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(trackSharpRocks, "objects/SharpRockMap.obj");
	m_Registry.emplace<syre::Transform>(trackSharpRocks, glm::vec3(-30.0f, 125.0f, -0.5f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(2.2f));
	m_Registry.emplace<syre::Texture>(trackSharpRocks, "images/SharpRock.png");

	//round rocks
	entt::entity trackRoundRocks = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(trackRoundRocks, "objects/RoundRockMap.obj");
	m_Registry.emplace<syre::Transform>(trackRoundRocks, glm::vec3(-30.0f, 125.0f, -0.5f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(2.2f));
	m_Registry.emplace<syre::Texture>(trackRoundRocks, "images/RoundRock.png");

	//flowers
	entt::entity trackFlowers = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(trackFlowers, "objects/FlowersMap.obj");
	m_Registry.emplace<syre::Transform>(trackFlowers, glm::vec3(-30.0f, 125.0f, -0.5f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(2.2f));
	m_Registry.emplace<syre::Texture>(trackFlowers, "images/Flower.png");

	//bush
	entt::entity trackBush = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(trackBush, "objects/BushMap.obj");
	m_Registry.emplace<syre::Transform>(trackBush, glm::vec3(-30.0f, 125.0f, -0.5f), glm::vec3(90.0f, 0.0f, 180.0f), glm::vec3(2.2f));
	m_Registry.emplace<syre::Texture>(trackBush, "images/Bush.png");

	entt::entity butterflies = m_Registry.create();
	m_Registry.emplace<syre::MorphRenderer>(butterflies);
	m_Registry.get<syre::MorphRenderer>(butterflies).AddFrame("ButterflyNeutral.obj");
	m_Registry.get<syre::MorphRenderer>(butterflies).AddFrame("ButterflyDown.obj");
	m_Registry.get<syre::MorphRenderer>(butterflies).AddFrame("ButterflyNeutral.obj");
	m_Registry.get<syre::MorphRenderer>(butterflies).AddFrame("ButterflyUp.obj");

	m_Registry.emplace<syre::Texture>(butterflies, "images/butterfly.png");
	m_Registry.emplace<syre::TransformList>(butterflies);
	m_Registry.get<syre::TransformList>(butterflies).SetDefaultRot(glm::vec3(90.0f, -70.0f, 180.0f));
	m_Registry.get<syre::TransformList>(butterflies).SetDefaultSca(glm::vec3(0.2f));
	m_Registry.get<syre::TransformList>(butterflies).AddPos(glm::vec3(0.0f, 0.0f, 0.0f));
	m_Registry.get<syre::TransformList>(butterflies).AddPos(glm::vec3(3.0f, -70.0f, 1.0f));
	m_Registry.get<syre::TransformList>(butterflies).AddPos(glm::vec3(35.0f, -97.f, 0.0f));
	m_Registry.get<syre::TransformList>(butterflies).AddPos(glm::vec3(110.0f, -104.f, 0.0f));
	m_Registry.get<syre::TransformList>(butterflies).AddPos(glm::vec3(153.0f, -205.f, 0.0f));

	entt::entity swayingTree = m_Registry.create();
	m_Registry.emplace<syre::MorphRenderer>(swayingTree);
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("TreeSwaying1.obj");
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("TreeSwaying2.obj");
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("TreeSwaying3.obj");
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("TreeSwaying4.obj");
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("TreeSwaying5.obj");
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("TreeSwaying6.obj");
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("TreeSwaying7.obj");
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("TreeSwaying8.obj");
	m_Registry.get<syre::MorphRenderer>(swayingTree).AddFrame("TreeSwaying9.obj");

	m_Registry.emplace<syre::Texture>(swayingTree, "images/Tree.png");
	m_Registry.emplace<syre::TransformList>(swayingTree);
	m_Registry.get<syre::TransformList>(swayingTree).SetDefaultRot(glm::vec3(90.0f, 0.0f, 0.0f));
	m_Registry.get<syre::TransformList>(swayingTree).SetDefaultSca(glm::vec3(1.0f));
	m_Registry.get<syre::TransformList>(swayingTree).AddPos(glm::vec3(-1.0f, -80.0f, 0.0f));

	*/





	m_Registry.emplace<Obstacles>(m_Obstacle);


	//remove this if frequent crashes
	/*m_Registry.emplace<syre::Mesh>(m_Particles1, "particleLol.obj");
	m_Registry.emplace<syre::Texture>(m_Particles1, "images/black.png");
	m_Registry.emplace<syre::TransformList>(m_Particles1);
	m_Registry.get<syre::TransformList>(m_Particles1).Particalize(0.05f, 0.6f);
	m_Registry.get<syre::TransformList>(m_Particles1).SetDefaultSca(glm::vec3(0.1f));

	m_Registry.emplace<syre::Mesh>(m_Particles2, "particleLol.obj");
	m_Registry.emplace<syre::Texture>(m_Particles2, "images/black.png");
	m_Registry.emplace<syre::TransformList>(m_Particles2);
	m_Registry.get<syre::TransformList>(m_Particles2).Particalize(0.05f, 0.6f);
	m_Registry.get<syre::TransformList>(m_Particles2).SetDefaultSca(glm::vec3(0.1f));*/


	m_Registry.emplace<Cars>(m_PCar);
	m_Registry.emplace<syre::Mesh>(m_PCar, "objects/Car2.obj");
	m_Registry.emplace<syre::Transform>(m_PCar, glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(90.f, 0.0f, 0.0f), glm::vec3(1.0f));
	m_Registry.emplace<syre::Texture>(m_PCar, "images/Car2.png");
	m_Registry.emplace<syre::PathAnimator>(m_PCar, syre::PathType::BEZIER);
	auto& carPath = m_Registry.get<syre::PathAnimator>(m_PCar);

	carPath.AddPoint(glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //start point
	carPath.AddPoint(glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(6.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(6.0f, -70.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //end of first straight
	carPath.AddPoint(glm::vec3(6.0f, -70.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(3.0f, -99.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-27.0f, -99.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //end of first turn
	carPath.AddPoint(glm::vec3(-27.0f, -99.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-27.0f, -99.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-97.0f, -99.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //str   
	carPath.AddPoint(glm::vec3(-97.0f, -99.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-139.0f, -99.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-139.0f, -129.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //half u   
	carPath.AddPoint(glm::vec3(-139.0f, -129.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-137.0f, -194.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 

	carPath.AddPoint(glm::vec3(-97.0f, -184.f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f)); //half u   
	carPath.AddPoint(glm::vec3(-97.0f, -184.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-97.0f, -184.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 

	carPath.AddPoint(glm::vec3(-18.0f, -184.f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f)); //str   
	carPath.AddPoint(glm::vec3(-18.0f, -184.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(17.0f, -176.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(8.0f, -246.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //turn   
	carPath.AddPoint(glm::vec3(8.0f, -246.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(8.0f, -246.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
							   
	carPath.AddPoint(glm::vec3(8.0f, -350.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //rocks
	carPath.AddPoint(glm::vec3(8.0f, -350.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(15.0f, -390.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-13.0f, -390.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //turn ///
	carPath.AddPoint(glm::vec3(-13.0f, -390.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-13.0f, -390.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-89.0f, -390.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //str 
	carPath.AddPoint(glm::vec3(-89.0f, -390.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-143.0f, -400.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-133.0f, -350.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //s 
	carPath.AddPoint(glm::vec3(-133.0f, -350.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-125.0f, -303.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-175.0f, -313.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //s
	carPath.AddPoint(glm::vec3(-175.0f, -313.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-175.0f, -313.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 

	carPath.AddPoint(glm::vec3(-250.0f, -313.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //rocks
	carPath.AddPoint(glm::vec3(-250.0f, -313.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-250.0f, -313.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-310.0f, -313.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //str
	carPath.AddPoint(glm::vec3(-310.0f, -313.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-350.0f, -313.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-346.0f, -273.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //u
	carPath.AddPoint(glm::vec3(-346.0f, -273.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-350.0f, -232.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-310.0f, -237.f, 0.0f), glm::vec3(90.0f, 0.0f, -270.0f)); //u
	carPath.AddPoint(glm::vec3(-310.0f, -237.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-310.0f, -237.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 

	carPath.AddPoint(glm::vec3(-235.0f, -237.f, 0.0f), glm::vec3(90.0f, 0.0f, -270.0f)); //str
	carPath.AddPoint(glm::vec3(-235.0f, -237.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-197.0f, -233.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-202.0f, -193.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //turn
	carPath.AddPoint(glm::vec3(-202.0f, -193.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-202.0f, -193.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 
								  
	carPath.AddPoint(glm::vec3(-202.0f, -60.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //str
	carPath.AddPoint(glm::vec3(-202.0f, -60.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-205.0f, -20.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-165.0f, -25.f, 0.0f), glm::vec3(90.0f, 0.0f, -270.0f)); //s
	carPath.AddPoint(glm::vec3(-165.0f, -25.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-118.0f, -30.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-125.0f, 20.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //s
	carPath.AddPoint(glm::vec3(-125.0f, 20.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-125.0f, 20.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-125.0f, 95.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //str
	carPath.AddPoint(glm::vec3(-125.0f, 95.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-120.0f, 125.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-155.0f, 125.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //turn
	carPath.AddPoint(glm::vec3(-155.0f, 125.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-155.0f, 125.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
										 
	carPath.AddPoint(glm::vec3(-230.0f, 125.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //str 
	carPath.AddPoint(glm::vec3(-230.0f, 125.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-230.0f, 125.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-300.0f, 125.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //rocks 
	carPath.AddPoint(glm::vec3(-300.0f, 125.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-300.0f, 125.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-365.0f, 125.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //str 
	carPath.AddPoint(glm::vec3(-365.0f, 125.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-408.5f, 130.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-408.5f, 95.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //turn 
	carPath.AddPoint(glm::vec3(-408.5f, 95.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-408.5f, 95.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 

	carPath.AddPoint(glm::vec3(-408.5f, 15.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //str
	carPath.AddPoint(glm::vec3(-408.5f, 15.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-405.0f, -29.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-375.0f, -24.f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f)); //half u
	carPath.AddPoint(glm::vec3(-375.0f, -24.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-320.0f, -32.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-325.0f, 12.f, 0.0f), glm::vec3(90.0f, 0.0f, 180.0f)); //end u
	carPath.AddPoint(glm::vec3(-325.0f, 12.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-330.0f, 60.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-285.0f, 56.f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f)); //start u
	carPath.AddPoint(glm::vec3(-285.0f, 56.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-245.0f, 55.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-250.0f, 20.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //end u
	carPath.AddPoint(glm::vec3(-250.0f, 20.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-250.0f, 20.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 
								
	carPath.AddPoint(glm::vec3(-250.0f, -120.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //str x2
	carPath.AddPoint(glm::vec3(-250.0f, -120.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-250.0f, -156.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-295.0f, -151.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //turn
	carPath.AddPoint(glm::vec3(-295.0f, -151.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-295.0f, -151.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 
										 
	carPath.AddPoint(glm::vec3(-425.0f, -151.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //str x2
	carPath.AddPoint(glm::vec3(-425.0f, -151.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-458.0f, -155.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-458.0f, -120.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //turn
	carPath.AddPoint(glm::vec3(-458.0f, -120.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-458.0f, -120.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 

	carPath.AddPoint(glm::vec3(-457.0f, 20.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //str x2
	carPath.AddPoint(glm::vec3(-457.0f, 20.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-450.0f, 57.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-495.0f, 57.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //s
	carPath.AddPoint(glm::vec3(-495.0f, 57.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-536.0f, 50.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-536.0f, 93.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //s
	carPath.AddPoint(glm::vec3(-536.0f, 93.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-540.0f, 139.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 

	carPath.AddPoint(glm::vec3(-574.0f, 134.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //start u
	carPath.AddPoint(glm::vec3(-574.0f, 134.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-627.0f, 140.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-622.0f, 93.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //end u
	carPath.AddPoint(glm::vec3(-622.0f, 93.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-622.0f, 93.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
								 
	carPath.AddPoint(glm::vec3(-622.0f, 20.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //str
	carPath.AddPoint(glm::vec3(-622.0f, 20.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-622.0f, 20.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
								 
	carPath.AddPoint(glm::vec3(-622.0f, -55.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //rocks
	carPath.AddPoint(glm::vec3(-622.0f, -55.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-622.0f, -55.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
									
	carPath.AddPoint(glm::vec3(-622.0f, -105.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //str
	carPath.AddPoint(glm::vec3(-622.0f, -105.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-620.0f, -160.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-574.0f, -149.f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f)); //s
	carPath.AddPoint(glm::vec3(-574.0f, -149.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-540.0f, -140.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.AddPoint(glm::vec3(-542.0f, -180.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //s
	carPath.AddPoint(glm::vec3(-542.0f, -180.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-542.0f, -180.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
								  
	carPath.AddPoint(glm::vec3(-542.0f, -300.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //str x2
	carPath.AddPoint(glm::vec3(-542.0f, -300.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	carPath.AddPoint(glm::vec3(-542.0f, -300.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	carPath.SpeedControl();
	

	m_Registry.emplace<Cars>(m_enemy);
	m_Registry.emplace<syre::Mesh>(m_enemy, "objects/Car2.obj");
	m_Registry.emplace<syre::Transform>(m_enemy, glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(90.f, 0.0f, 0.0f), glm::vec3(1.0f));
	m_Registry.emplace<syre::Texture>(m_enemy, "images/Car2-Blue.png");
	m_Registry.emplace<syre::PathAnimator>(m_enemy, syre::PathType::BEZIER);
	auto& enemyCarPath = m_Registry.get<syre::PathAnimator>(m_enemy);

	enemyCarPath.AddPoint(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //start point
	enemyCarPath.AddPoint(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(10.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(10.0f, -70.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //end of first straight
	enemyCarPath.AddPoint(glm::vec3(10.0f, -70.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(7.0f, -100.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	enemyCarPath.AddPoint(glm::vec3(-27.0f, -103.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //end of first turn
	enemyCarPath.AddPoint(glm::vec3(-27.0f, -103.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-27.0f, -103.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-97.0f, -103.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //str   
	enemyCarPath.AddPoint(glm::vec3(-97.0f, -103.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-139.0f, -103.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-135.0f, -129.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //half u   
	enemyCarPath.AddPoint(glm::vec3(-135.0f, -129.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-137.0f, -190.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-97.0f, -180.f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f)); //half u   
	enemyCarPath.AddPoint(glm::vec3(-97.0f, -180.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-97.0f, -180.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-18.0f, -180.f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f)); //str   
	enemyCarPath.AddPoint(glm::vec3(-18.0f, -180.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(20.0f, -172.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(12.0f, -256.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //turn   
	enemyCarPath.AddPoint(glm::vec3(12.0f, -256.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(12.0f, -256.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(12.0f, -350.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //rocks
	enemyCarPath.AddPoint(glm::vec3(12.0f, -350.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(19.0f, -390.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-13.0f, -394.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //turn
	enemyCarPath.AddPoint(glm::vec3(-13.0f, -394.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-13.0f, -394.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)); ////////////////
	
	enemyCarPath.AddPoint(glm::vec3(-89.0f, -394.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //str
	enemyCarPath.AddPoint(glm::vec3(-89.0f, -394.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-143.0f, -404.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-137.0f, -350.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //s 
	enemyCarPath.AddPoint(glm::vec3(-137.0f, -350.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-125.0f, -303.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-175.0f, -317.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //s
	enemyCarPath.AddPoint(glm::vec3(-175.0f, -317.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-179.0f, -317.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-250.0f, -317.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //rocks
	enemyCarPath.AddPoint(glm::vec3(-250.0f, -317.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-250.0f, -317.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
								   
	enemyCarPath.AddPoint(glm::vec3(-310.0f, -317.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //str
	enemyCarPath.AddPoint(glm::vec3(-310.0f, -317.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-350.0f, -317.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-350.0f, -273.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //u
	enemyCarPath.AddPoint(glm::vec3(-350.0f, -273.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-350.0f, -228.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-310.0f, -233.f, 0.0f), glm::vec3(90.0f, 0.0f, -270.0f)); //u
	enemyCarPath.AddPoint(glm::vec3(-310.0f, -233.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-310.0f, -233.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-235.0f, -233.f, 0.0f), glm::vec3(90.0f, 0.0f, -270.0f)); //str
	enemyCarPath.AddPoint(glm::vec3(-235.0f, -233.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-197.0f, -229.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-206.0f, -193.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //turn
	enemyCarPath.AddPoint(glm::vec3(-206.0f, -193.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-206.0f, -193.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-206.0f, -60.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //str x2
	enemyCarPath.AddPoint(glm::vec3(-206.0f, -60.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-209.0f, -20.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)); //////
	
	enemyCarPath.AddPoint(glm::vec3(-165.0f, -21.f, 0.0f), glm::vec3(90.0f, 0.0f, -270.0f)); //s
	enemyCarPath.AddPoint(glm::vec3(-165.0f, -21.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-118.0f, -26.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-129.0f, 20.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //s
	enemyCarPath.AddPoint(glm::vec3(-129.0f, 20.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-129.0f, 20.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-129.0f, 95.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //str
	enemyCarPath.AddPoint(glm::vec3(-129.0f, 95.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-124.0f, 125.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-155.0f, 121.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //turn
	enemyCarPath.AddPoint(glm::vec3(-155.0f, 121.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-155.0f, 121.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
								  
	enemyCarPath.AddPoint(glm::vec3(-230.0f, 121.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //str 
	enemyCarPath.AddPoint(glm::vec3(-230.0f, 121.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-230.0f, 121.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
								  
	enemyCarPath.AddPoint(glm::vec3(-300.0f, 121.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //rocks 
	enemyCarPath.AddPoint(glm::vec3(-300.0f, 121.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-300.0f, 121.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
								  
	enemyCarPath.AddPoint(glm::vec3(-365.0f, 121.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //str 
	enemyCarPath.AddPoint(glm::vec3(-365.0f, 121.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-408.0f, 126.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-404.5f, 95.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //turn 
	enemyCarPath.AddPoint(glm::vec3(-404.5f, 95.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-404.5f, 95.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
						  
	enemyCarPath.AddPoint(glm::vec3(-404.5f, 15.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //str
	enemyCarPath.AddPoint(glm::vec3(-404.5f, 15.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-401.f, -29.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-375.0f, -20.f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f)); //half u
	enemyCarPath.AddPoint(glm::vec3(-375.0f, -20.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-324.0f, -32.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-329.0f, 12.f, 0.0f), glm::vec3(90.0f, 0.0f, 180.0f)); //end u
	enemyCarPath.AddPoint(glm::vec3(-329.0f, 12.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-334.0f, 60.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-285.0f, 60.f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f)); //start u
	enemyCarPath.AddPoint(glm::vec3(-285.0f, 60.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-241.0f, 55.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-246.0f, 20.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //end u
	enemyCarPath.AddPoint(glm::vec3(-246.0f, 20.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-246.0f, 20.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
						 
	enemyCarPath.AddPoint(glm::vec3(-246.0f, -120.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //str x2
	enemyCarPath.AddPoint(glm::vec3(-246.0f, -120.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-246.0f, -156.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-295.0f, -155.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //turn
	enemyCarPath.AddPoint(glm::vec3(-295.0f, -155.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-295.0f, -155.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-425.0f, -155.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //str x2
	enemyCarPath.AddPoint(glm::vec3(-425.0f, -155.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-458.0f, -159.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-463.0f, -120.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //turn
	enemyCarPath.AddPoint(glm::vec3(-463.0f, -120.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-463.0f, -120.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
						 
	enemyCarPath.AddPoint(glm::vec3(-463.0f, 20.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //str x2
	enemyCarPath.AddPoint(glm::vec3(-463.0f, 20.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-454.0f, 57.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-495.0f, 53.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //s
	enemyCarPath.AddPoint(glm::vec3(-495.0f, 53.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-540.0f, 50.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-540.0f, 93.f, 0.0f), glm::vec3(90.0f, 0.0f, -180.0f)); //s
	enemyCarPath.AddPoint(glm::vec3(-540.0f, 93.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-540.0f, 135.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-574.0f, 130.f, 0.0f), glm::vec3(90.0f, 0.0f, -90.0f)); //start u
	enemyCarPath.AddPoint(glm::vec3(-574.0f, 130.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-623.0f, 140.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-618.0f, 93.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //end u
	enemyCarPath.AddPoint(glm::vec3(-618.0f, 93.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-618.0f, 93.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
						 
	enemyCarPath.AddPoint(glm::vec3(-618.0f, 20.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //str
	enemyCarPath.AddPoint(glm::vec3(-618.0f, 20.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-618.0f, 20.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
						 
	enemyCarPath.AddPoint(glm::vec3(-618.0f, -55.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //rocks
	enemyCarPath.AddPoint(glm::vec3(-618.0f, -55.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-618.0f, -55.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
						 
	enemyCarPath.AddPoint(glm::vec3(-618.0f, -105.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //str
	enemyCarPath.AddPoint(glm::vec3(-618.0f, -105.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-616.0f, -160.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-574.0f, -145.f, 0.0f), glm::vec3(90.0f, 0.0f, 90.0f)); //s
	enemyCarPath.AddPoint(glm::vec3(-574.0f, -145.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-536.0f, -14.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.AddPoint(glm::vec3(-538.0f, -180.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //s
	enemyCarPath.AddPoint(glm::vec3(-538.0f, -180.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-538.0f, -180.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
						 
	enemyCarPath.AddPoint(glm::vec3(-538.0f, -300.f, 0.0f), glm::vec3(90.0f, 0.0f, 0.0f)); //str x2
	enemyCarPath.AddPoint(glm::vec3(-538.0f, -300.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	enemyCarPath.AddPoint(glm::vec3(-538.0f, -300.f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	
	enemyCarPath.SpeedControl();
	
	//cards

	m_Registry.emplace<syre::Mesh>(m_Card, "objects/Card.obj");
	m_Registry.emplace<syre::Transform>(m_Card, glm::vec3(-3.0f, 0.0f, 2.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2f));
	m_Registry.emplace<syre::Texture>(m_Card, "images/NO2.png");



	cardTextures.push_back(syre::Texture("images/NO2.png"));
	cardTextures.push_back(syre::Texture("images/Drift.png"));
	cardTextures.push_back(syre::Texture("images/Slipstream.png"));
	cardTextures.push_back(syre::Texture("images/EBrake.png"));
	cardTextures.push_back(syre::Texture("images/QuickShift.png"));
	cardTextures.push_back(syre::Texture("images/Sabotage.png"));


	hazardTextures.push_back(syre::Texture("images/Apex.png"));
	hazardTextures.push_back(syre::Texture("images/Hairpin.png"));
	hazardTextures.push_back(syre::Texture("images/Chicane.png"));
	hazardTextures.push_back(syre::Texture("images/Rocks.png"));

	gearboxTextures.push_back(syre::Texture("images/GearBoxNeutral.png"));
	gearboxTextures.push_back(syre::Texture("images/GearBoxGasPressed.png"));
	gearboxTextures.push_back(syre::Texture("images/GearBoxBrakePressed.png"));

	accelerometerTextures.push_back(syre::Texture("images/Accelerometer1.png"));
	accelerometerTextures.push_back(syre::Texture("images/Accelerometer1.png"));
	accelerometerTextures.push_back(syre::Texture("images/Accelerometer2.png"));
	accelerometerTextures.push_back(syre::Texture("images/Accelerometer3.png"));
	accelerometerTextures.push_back(syre::Texture("images/Accelerometer4.png"));
	accelerometerTextures.push_back(syre::Texture("images/Accelerometer5.png"));
	accelerometerTextures.push_back(syre::Texture("images/Accelerometer6.png"));

	flatShader = Shader::Create();
	flatShader->LoadShaderPartFromFile("flatVert.glsl", GL_VERTEX_SHADER);
	flatShader->LoadShaderPartFromFile("flatFrag.glsl", GL_FRAGMENT_SHADER);
	flatShader->Link();






	basicShader = Shader::Create();
	basicShader->LoadShaderPartFromFile("vertex_shader.glsl", GL_VERTEX_SHADER);
	basicShader->LoadShaderPartFromFile("frag_shader.glsl", GL_FRAGMENT_SHADER);
	basicShader->Link();

	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 2.0f);
	glm::vec3 lightCol = glm::vec3(1.0f, 1.0f, 1.0f);
	float     lightAmbientPow = 0.1f;
	float     lightSpecularPow = 1.0f;
	glm::vec3 ambientCol = glm::vec3(1.0f);
	float     ambientPow = 0.5f;
	float     shininess = 4.0f;
	// These are our application / scene level uniforms that don't necessarily update
	// every frame
	basicShader->SetUniform("u_LightPos", lightPos);
	basicShader->SetUniform("u_LightCol", lightCol);
	basicShader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
	basicShader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
	basicShader->SetUniform("u_AmbientCol", ambientCol);
	basicShader->SetUniform("u_AmbientStrength", ambientPow);
	basicShader->SetUniform("u_Shininess", shininess);


	morphShader = Shader::Create();
	morphShader->LoadShaderPartFromFile("morph_vertex_shader.glsl", GL_VERTEX_SHADER);
	morphShader->LoadShaderPartFromFile("frag_shader.glsl", GL_FRAGMENT_SHADER);
	morphShader->Link();

	lightPos = glm::vec3(0.0f, 0.0f, 2.0f);
	lightCol = glm::vec3(1.0f, 1.0f, 1.0f);
	lightAmbientPow = 0.1f;
	lightSpecularPow = 1.0f;
	ambientCol = glm::vec3(1.0f);
	ambientPow = 0.5f;
	shininess = 4.0f;
	// These are our application / scene level uniforms that don't necessarily update
	// every frame
	morphShader->SetUniform("u_LightPos", lightPos);
	morphShader->SetUniform("u_LightCol", lightCol);
	morphShader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
	morphShader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
	morphShader->SetUniform("u_AmbientCol", ambientCol);
	morphShader->SetUniform("u_AmbientStrength", ambientPow);
	morphShader->SetUniform("u_Shininess", shininess);


	flatMorphShader = Shader::Create();
	flatMorphShader->LoadShaderPartFromFile("flatMorphVert.glsl", GL_VERTEX_SHADER);
	flatMorphShader->LoadShaderPartFromFile("flatFrag.glsl", GL_FRAGMENT_SHADER);
	flatMorphShader->Link();


	auto& camComponent = camera;
	camComponent->SetPosition(glm::vec3(0, 3, 3)); // Set initial position
	camComponent->SetUp(glm::vec3(0, 0, 1)); // Use a z-up coordinate system
	camComponent->LookAt(glm::vec3(0.0f)); // Look at center of the screen
	camComponent->SetFovDegrees(100.0f); // Set an initial FOV

	auto& obstacleComponent = m_Registry.get<Obstacles>(m_Obstacle);

	auto listView = m_Registry.view<syre::TransformList>();
	for (auto entity : listView)
	{
		m_Registry.get<syre::TransformList>(entity).SetCamera(camera);
	}


	lastFrame = glfwGetTime();
}

int Scene2::Update()
{

	AudioEngine& engine = AudioEngine::Instance();

	AudioEvent& ambient = engine.GetEvent("Ambient");
	AudioBus& musicBus = engine.GetBus("Music");


	//get ref listener
	AudioListener& listener = engine.GetListener();
	listener.SetPosition(glm::vec3(5, 0, 0));

	engine.Update();

	if (isPaused)
	{
		return PausedUpdate();
	}
	thisFrame = glfwGetTime();
	float deltaTime = thisFrame - lastFrame;
	auto& camComponent = camera;
	auto& obstacleComponent = m_Registry.get<Obstacles>(m_Obstacle);
	auto& PlayerComponent = m_Registry.get<Cars>(m_PCar);
	auto& EnemyComponent = m_Registry.get<Cars>(m_enemy);
	int temp = 0;
	bool done = false;
	bool Pemp = false;
	bool Eemp = false;
	if (start2 == 0)
	{
		start2 += 1;
		obstacleComponent.Draw();
	}
	glm::vec3 camX = glm::cross(camComponent->GetForward(), camComponent->GetUp());
	KeyEvents(deltaTime);
	flatShader->Bind();
	flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.115f)));
	flatShader->SetUniform("aspect", camera->GetAspect());
	for (int i = 0; i <= 4; i++)
	{
		int cardVal = PlayerComponent.GetCard(i, true);
		if (cardVal != -1)
		{
			flatShader->SetUniform("offset", glm::vec2(-0.1f + i / 4.2f, -.66f));

			cardTextures[cardVal].Bind();
			m_Registry.get<syre::Mesh>(m_Card).Render();
		}
	}


	flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.09f)));
	flatShader->SetUniform("offset", glm::vec2(0.90, 0.8f));
	int ObsVal = obstacleComponent.GetObs();
	hazardTextures[ObsVal].Bind();
	m_Registry.get<syre::Mesh>(m_Hazard).Render();

	flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)));
	flatShader->SetUniform("offset", glm::vec2(-0.55f, -0.95f));
	int GerVal = PlayerComponent.GetGear();
	accelerometerTextures[GerVal].Bind();
	m_Registry.get<syre::Mesh>(m_Accelerometer).Render();

	if (lbutton_down == false)
	{
		flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.125f)));
		flatShader->SetUniform("offset", glm::vec2(-0.87, -0.75f));
		m_Registry.get<syre::Texture>(m_GearboxLever).Bind();
		m_Registry.get<syre::Mesh>(m_GearboxLever).Render();
	}
	if (lbutton_down == true)
	{
		double* x = new double;
		double* y = new double;

		glfwGetCursorPos(window, x, y);
		if (-0.75f + 0.6f + (*y * -0.00104f) >= -0.79 && -0.75f + 0.6f + (*y * -0.00104f) <= -0.71)
		{
			flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.125f)));
			flatShader->SetUniform("offset", glm::vec2(-0.87, -0.75f + 0.6f + (*y * -0.00104f)));
			m_Registry.get<syre::Texture>(m_GearboxLever).Bind();
			m_Registry.get<syre::Mesh>(m_GearboxLever).Render();
		}
		else if (-0.75f + 0.6f + (*y * -0.00104f) <= -0.79 && PlayerComponent.GetBrake() == true)
		{
			PlayerComponent.SetAction(-4);
			PlayerComponent.ChangeGears();
			PlayerComponent.ResetPed();
			lbutton_down = false;
		}
		else if (-0.75f + 0.6f + (*y * -0.00104f) >= -0.71 && PlayerComponent.GetAcc() == true)
		{
			PlayerComponent.SetAction(-5);
			PlayerComponent.ChangeGears();
			PlayerComponent.ResetPed();
			lbutton_down = false;
		}
		else
		{
			lbutton_down = false;
		}
	}


	bool Accelerate = PlayerComponent.GetAcc();
	bool Brake = PlayerComponent.GetBrake();
	if (Accelerate == false && Brake == false)
	{
		flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.125f)));
		flatShader->SetUniform("offset", glm::vec2(-0.87, -0.75f));
		gearboxTextures[0].Bind();
		m_Registry.get<syre::Mesh>(m_Gearbox).Render();
	}
	else if (Brake == true)
	{
		flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.125f)));
		flatShader->SetUniform("offset", glm::vec2(-0.87, -0.75f));
		gearboxTextures[2].Bind();
		m_Registry.get<syre::Mesh>(m_Gearbox).Render();
	}
	else if (Accelerate == true)
	{
		flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.125f)));
		flatShader->SetUniform("offset", glm::vec2(-0.87, -0.75f));
		gearboxTextures[1].Bind();
		m_Registry.get<syre::Mesh>(m_Gearbox).Render();
	}

	if (m_Registry.get<syre::PathAnimator>(m_PCar).HitMax() || m_Registry.get<syre::PathAnimator>(m_enemy).HitMax())
	{
		m_Registry.get<syre::PathAnimator>(m_PCar).Stop();
		m_Registry.get<syre::PathAnimator>(m_enemy).Stop();
	}
	else if (!(m_Registry.get<syre::PathAnimator>(m_PCar).HitMax() && m_Registry.get<syre::PathAnimator>(m_enemy).HitMax()))
	{
		m_Registry.get<syre::PathAnimator>(m_PCar).Resume();
		m_Registry.get<syre::PathAnimator>(m_enemy).Resume();
	}
	if (obstacleComponent.GetEnd() != true)
	{
		if (EnemyComponent.GetAction1() == -1 && EnemyComponent.GetAction2() == -1)
		{
			if (speedDemon == true)
			{
				for (int i = 0; i <= 5; i++)
				{
					if (EnemyComponent.GetCard(i, true) == 0)
					{
						EnemyComponent.PlayCard(i, 0);
						break;
					}
				}
				if (EnemyComponent.GetAction1() == -1 && EnemyComponent.GetAction2() == -1)
				{
					EnemyComponent.SetAcc();
					EnemyComponent.ChangeGears();
				}
				else if (EnemyComponent.GetAction2() == -1)
				{
					if (EnemyComponent.GetAcc() == false)
					{
						EnemyComponent.SetAcc();
					}
					else if (EnemyComponent.GetAcc() == true)
					{
						EnemyComponent.ChangeGears();
					}
				}
				if (EnemyComponent.GetGear() == 6)
				{
					speedDemon = false;
				}
			}
			else if (speedDemon == false)
			{
				if (EnemyComponent.GetGear() == 1)
				{
					speedDemon = true;
				}
				EnemyComponent.SetBrk();
				EnemyComponent.ChangeGears();

			}
		}
		if (showGear == false)
		{
			showGear = true;
			printf("The enemy's gear level is : ");
			printf("%i", EnemyComponent.GetGear());
			printf("\n");
			printf("\n");
		}

		if (PlayerComponent.GetAction1() != -1 && PlayerComponent.GetAction2() != -1)
		{
			/// harry i changed the bit below, what did it do lol
			for (int i = 0; i <= 5; i++)
			{
				if (PlayerComponent.GetCard(i, true) == -1)
				{
					temp += 1;
				}
				if (temp == 6)//changed from = to ==
				{
					Pemp = true;
					temp = 0;
				}
			}
			for (int i = 0; i <= 5; i++)
			{
				if (EnemyComponent.GetCard(i, true) == -1)
				{
					temp += 1;
				}
				if (temp == 6)//changed from = to ==
				{
					Eemp = true;
					temp = 0;
				}
			}
			if (obstacleComponent.GetObs() != 2)
			{
				if (obstacleComponent.Resolve(PlayerComponent.GetGear(), EnemyComponent.GetGear()) == 1)
				{
					PlayerComponent.IncreaseScore();
					m_Registry.get<syre::PathAnimator>(m_PCar).SetSpeed(0.25, true);
					m_Registry.get<syre::PathAnimator>(m_enemy).SetSpeed(0.25, false);
					if (obstacleComponent.GetObs() == 3)
					{
						if (Eemp != true)
						{
							while (done != true)
							{
								srand((unsigned)time(0));
								temp = (rand() % 5);
								if (EnemyComponent.GetCard(temp, true) != -1)
								{
									EnemyComponent.RemoveCard(temp, true);
									break;
								}
							}
						}
					}
				}
				else
				{
					EnemyComponent.IncreaseScore();
					m_Registry.get<syre::PathAnimator>(m_PCar).SetSpeed(0.25, false);
					m_Registry.get<syre::PathAnimator>(m_enemy).SetSpeed(0.25, true);
					if (obstacleComponent.GetObs() == 3)
					{
						if (Pemp != true)
						{
							while (done != true)
							{
								srand((unsigned)time(0));
								temp = (rand() % 5);

								if (PlayerComponent.GetCard(temp, true) != -1)
								{
									PlayerComponent.RemoveCard(temp, true);
									break;
								}
							}
						}
					}
				}
			}
			if (EnemyComponent.GetAction1() == 1 || EnemyComponent.GetAction2() == 1)
			{
				temp = EnemyComponent.GetGear();
				EnemyComponent.ChangeGears(PlayerComponent.GetGear());
				PlayerComponent.ChangeGears(temp);
			}
			if (PlayerComponent.GetAction1() == 1 || PlayerComponent.GetAction2() == 1)
			{
				temp = PlayerComponent.GetGear();
				PlayerComponent.ChangeGears(EnemyComponent.GetGear());
				EnemyComponent.ChangeGears(temp);
			}
			if (obstacleComponent.GetObs() == 2)
			{
				obstacleComponent.Resolve(PlayerComponent.GetGear(), EnemyComponent.GetGear());
				if (obstacleComponent.Resolve(PlayerComponent.GetGear(), EnemyComponent.GetGear() == true))
				{
					PlayerComponent.IncreaseScore();
					m_Registry.get<syre::PathAnimator>(m_PCar).SetSpeed(0.25, true);
					m_Registry.get<syre::PathAnimator>(m_enemy).SetSpeed(0.25, false);
				}
				else
				{
					EnemyComponent.IncreaseScore();
					m_Registry.get<syre::PathAnimator>(m_PCar).SetSpeed(0.25, false);
					m_Registry.get<syre::PathAnimator>(m_enemy).SetSpeed(0.25, true);
				}
			}
			if (obstacleComponent.GetObs() != 2 && obstacleComponent.GetObs() != 3)
			{
				m_Registry.get<syre::PathAnimator>(m_PCar).IncrementSegment(2);
				m_Registry.get<syre::PathAnimator>(m_enemy).IncrementSegment(2);
			}
			else
			{
				m_Registry.get<syre::PathAnimator>(m_PCar).IncrementSegment(1);
				m_Registry.get<syre::PathAnimator>(m_enemy).IncrementSegment(1);
			}
			PlayerComponent.ResetTurn();
			EnemyComponent.ResetTurn();
			obstacleComponent.Draw();
			showGear = false;
			if (obstacleComponent.GetObs() == 2)
			{
				PlayerComponent.SetAction(-6);
				EnemyComponent.SetAction(-6);
			}
		}
	}
	else
	{
		m_Registry.get<syre::PathAnimator>(m_PCar).IncrementSegment(2);//needs changed
		m_Registry.get<syre::PathAnimator>(m_enemy).IncrementSegment(2);//needs changed
		if (PlayerComponent.GetScore() >= EnemyComponent.GetScore())
		{
			printf("PLAYER WINS");
			m_Registry.get<syre::PathAnimator>(m_PCar).SetSpeed(1.0, true);
			m_Registry.get<syre::PathAnimator>(m_enemy).SetSpeed(1.0, false);
			if (m_Registry.get<syre::PathAnimator>(m_PCar).GetHardStop())
			{
				//game is finished finished
				m_Registry.get<syre::PathAnimator>(m_enemy).Stop();
				//this is where we could go to next level
			}
		}
		else if (PlayerComponent.GetScore() < EnemyComponent.GetScore())
		{
			printf("ENEMY WINS");
			m_Registry.get<syre::PathAnimator>(m_PCar).SetSpeed(1.0, false);
			m_Registry.get<syre::PathAnimator>(m_enemy).SetSpeed(1.0, true);
		}
	}
	flatMorphShader->Bind();
	flatMorphShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.12f)));
	flatMorphShader->SetUniform("offset", glm::vec2(-0.55, -0.97f));
	flatMorphShader->SetUniform("t", 0.0f);
	m_Registry.get<syre::MorphRenderer>(m_Needle).ManualFrameSet(0);
	m_Registry.get<syre::Texture>(m_Needle).Bind();
	m_Registry.get<syre::MorphRenderer>(m_Needle).Render();
	auto pathView = m_Registry.view<syre::PathAnimator, syre::Transform>();
	for (auto entity : pathView)
	{
		auto& transform = m_Registry.get<syre::Transform>(entity);
		m_Registry.get<syre::PathAnimator>(entity).Update(transform, deltaTime);
	}


	basicShader->Bind();
	basicShader->SetUniform("u_CamPos", camComponent->GetPosition());
	basicShader->SetUniform("playerPos", m_Registry.get<syre::Transform>(m_PCar).GetPosition());
	basicShader->SetUniform("enemyPos", m_Registry.get<syre::Transform>(m_enemy).GetPosition());

	auto renderView = m_Registry.view<syre::Mesh, syre::Transform, syre::Texture>();
	for (auto entity : renderView)
	{
		glm::mat4 transform = renderView.get<syre::Transform>(entity).GetModelMat();
		basicShader->SetUniformMatrix("u_ModelViewProjection", camComponent->GetViewProjection() * transform);
		basicShader->SetUniformMatrix("u_Model", transform);
		basicShader->SetUniformMatrix("u_ModelRotation", glm::mat3(transform));
		renderView.get<syre::Texture>(entity).Bind();
		renderView.get<syre::Mesh>(entity).Render();
	}
	auto listRenderView = m_Registry.view<syre::Mesh, syre::TransformList, syre::Texture>();
	for (auto entity : listRenderView)
	{
		listRenderView.get<syre::Texture>(entity).Bind();
		listRenderView.get<syre::TransformList>(entity).ListRender(basicShader, listRenderView.get<syre::Mesh>(entity), deltaTime);
	}

	auto morphRenderView = m_Registry.view<syre::MorphRenderer, syre::Transform, syre::Texture>();
	morphShader->SetUniform("u_CamPos", camComponent->GetPosition());
	morphShader->SetUniform("playerPos", m_Registry.get<syre::Transform>(m_PCar).GetPosition());
	morphShader->SetUniform("enemyPos", m_Registry.get<syre::Transform>(m_enemy).GetPosition());
	morphShader->Bind();
	for (auto entity : morphRenderView)
	{
		float t = morphRenderView.get<syre::MorphRenderer>(entity).Update(deltaTime);
		glm::mat4 transform = morphRenderView.get<syre::Transform>(entity).GetModelMat();
		morphShader->SetUniformMatrix("u_ModelViewProjection", camComponent->GetViewProjection() * transform);
		morphShader->SetUniformMatrix("u_Model", transform);
		morphShader->SetUniformMatrix("u_ModelRotation", glm::mat3(transform));
		morphShader->SetUniform("t", t);
		morphRenderView.get<syre::Texture>(entity).Bind();
		morphRenderView.get<syre::MorphRenderer>(entity).Render();
	}
	auto morphListRenderView = m_Registry.view<syre::MorphRenderer, syre::TransformList, syre::Texture>();
	for (auto entity : morphListRenderView)
	{
		float t = morphListRenderView.get<syre::MorphRenderer>(entity).Update(deltaTime);
		morphShader->SetUniform("t", t);
		morphListRenderView.get<syre::Texture>(entity).Bind();
		morphListRenderView.get<syre::TransformList>(entity).ListRender(morphShader, morphListRenderView.get<syre::MorphRenderer>(entity));
	}
	if (!manualCamera)
	{
		camComponent->SetPosition(m_Registry.get<syre::Transform>(m_PCar).GetPosition() + glm::vec3(1.0f, 4.0f, 5.0f));
	}
	camComponent->SetForward(glm::normalize(m_Registry.get<syre::Transform>(m_PCar).GetPosition() - camComponent->GetPosition()));
	/*m_Registry.get<syre::TransformList>(m_Particles1).UpdateCurPos(m_Registry.get<syre::Transform>(m_PCar).GetPosition());
	m_Registry.get<syre::TransformList>(m_Particles2).UpdateCurPos(m_Registry.get<syre::Transform>(m_enemy).GetPosition());*/

	lastFrame = thisFrame;
	return 0;
}

int Scene2::PausedUpdate()
{
	thisFrame = glfwGetTime();
	float deltaTime = thisFrame - lastFrame;
	auto& camComponent = camera;
	auto& obstacleComponent = m_Registry.get<Obstacles>(m_Obstacle);
	auto& PlayerComponent = m_Registry.get<Cars>(m_PCar);
	auto& EnemyComponent = m_Registry.get<Cars>(m_enemy);
	int returning = KeyEvents(deltaTime);




	basicShader->Bind();
	basicShader->SetUniform("u_CamPos", camComponent->GetPosition());
	basicShader->SetUniform("playerPos", m_Registry.get<syre::Transform>(m_PCar).GetPosition());
	basicShader->SetUniform("enemyPos", m_Registry.get<syre::Transform>(m_enemy).GetPosition());

	auto renderView = m_Registry.view<syre::Mesh, syre::Transform, syre::Texture>();
	for (auto entity : renderView)
	{
		glm::mat4 transform = renderView.get<syre::Transform>(entity).GetModelMat();
		basicShader->SetUniformMatrix("u_ModelViewProjection", camComponent->GetViewProjection() * transform);
		basicShader->SetUniformMatrix("u_Model", transform);
		basicShader->SetUniformMatrix("u_ModelRotation", glm::mat3(transform));
		renderView.get<syre::Texture>(entity).Bind();
		renderView.get<syre::Mesh>(entity).Render();
	}
	auto listRenderView = m_Registry.view<syre::Mesh, syre::TransformList, syre::Texture>();
	for (auto entity : listRenderView)
	{
		listRenderView.get<syre::Texture>(entity).Bind();
		listRenderView.get<syre::TransformList>(entity).ListRender(basicShader, listRenderView.get<syre::Mesh>(entity), deltaTime);
	}
	auto morphRenderView = m_Registry.view<syre::MorphRenderer, syre::Transform, syre::Texture>();
	morphShader->SetUniform("u_CamPos", camComponent->GetPosition());
	morphShader->SetUniform("playerPos", m_Registry.get<syre::Transform>(m_PCar).GetPosition());
	morphShader->SetUniform("enemyPos", m_Registry.get<syre::Transform>(m_enemy).GetPosition());
	morphShader->Bind();
	for (auto entity : morphRenderView)
	{
		float t = morphRenderView.get<syre::MorphRenderer>(entity).GetT();
		glm::mat4 transform = morphRenderView.get<syre::Transform>(entity).GetModelMat();
		morphShader->SetUniformMatrix("u_ModelViewProjection", camComponent->GetViewProjection() * transform);
		morphShader->SetUniformMatrix("u_Model", transform);
		morphShader->SetUniformMatrix("u_ModelRotation", glm::mat3(transform));
		morphShader->SetUniform("t", t);
		morphRenderView.get<syre::Texture>(entity).Bind();
		morphRenderView.get<syre::MorphRenderer>(entity).Render();
	}
	auto morphListRenderView = m_Registry.view<syre::MorphRenderer, syre::TransformList, syre::Texture>();
	for (auto entity : morphListRenderView)
	{
		float t = morphListRenderView.get<syre::MorphRenderer>(entity).GetT();
		morphShader->SetUniform("t", t);
		morphListRenderView.get<syre::Texture>(entity).Bind();
		morphListRenderView.get<syre::TransformList>(entity).ListRender(morphShader, morphListRenderView.get<syre::MorphRenderer>(entity));
	}

	if (!manualCamera)
	{
		camComponent->SetPosition(m_Registry.get<syre::Transform>(m_PCar).GetPosition() + glm::vec3(1.0f, 4.0f, 5.0f));
	}
	camComponent->SetForward(glm::normalize(m_Registry.get<syre::Transform>(m_PCar).GetPosition() - camComponent->GetPosition()));

	flatShader->Bind();
	flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(0.2f)));
	flatShader->SetUniform("offset", glm::vec2(0.0, 0.0f));
	flatShader->SetUniform("aspect", camera->GetAspect());
	m_Registry.get<syre::Texture>(m_PauseMenu).Bind();
	m_Registry.get<syre::Mesh>(m_PauseMenu).Render();

	flatShader->SetUniformMatrix("scale", glm::scale(glm::mat4(1.0f), glm::vec3(1.2f)));
	flatShader->SetUniform("offset", glm::vec2(0.0, 0.0f));
	flatShader->SetUniform("aspect", camera->GetAspect());
	m_Registry.get<syre::Texture>(m_TransparentBlack).Bind();
	m_Registry.get<syre::Mesh>(m_TransparentBlack).Render();


	lastFrame = thisFrame;
	return returning;
}

void Scene2::ImGUIUpdate()
{
	//auto& PlayerComponent = m_Registry.get<Cars>(m_PCar);
		// Implementation new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		// ImGui context new frame
		ImGui::NewFrame();

		if (ImGui::Begin("Debug")) {
			// Render our GUI stuff


			auto movable = m_Registry.view<syre::Mesh, syre::Transform>();
			auto& camComponent = camera;
			glm::vec3 camPos = camComponent->GetPosition();
			if (ImGui::Button(manualCamera?"Auto Camera": "Manual Camera"))
			{
				manualCamera = !manualCamera;
			}
			if (manualCamera)
			{
				ImGui::SliderFloat3("Camera Position", &camPos.x,-200.f, 200.f);
			}
			camComponent->SetPosition(camPos);
			ImGui::End();
		}

		// Make sure ImGui knows how big our window is
		ImGuiIO& io = ImGui::GetIO();
		int width{ 0 }, height{ 0 };
		glfwGetWindowSize(window, &width, &height);
		io.DisplaySize = ImVec2((float)width, (float)height);

		// Render all of our ImGui elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			// Update the windows that ImGui is using
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			// Restore our gl context
			glfwMakeContextCurrent(window);
		}

}

Camera::sptr& Scene2::GetCam()
{
	// TODO: insert return statement here
	return camera;
}

int Scene2::KeyEvents(float delta)
{
	if (isPaused)
	{
		double* x = new double;
		double* y = new double;

		glfwGetCursorPos(window, x, y);
		//printf("Mouse at X %f Y %f\n", *x, *y);

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
		{
			escRelease = true;
		}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && escRelease)
		{
			isPaused = false;
			escRelease = false;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			if (552.0f < *x && *x < 729.0f)
			{
				if (323.0f < *y && *y < 367.0f)
				{
					isPaused = false;
				}
				else if (374.0f < *y && *y < 417.0f)
				{
					return -1;
				}
				else if (425.0f < *y && *y < 474.0f)
				{
					return -2;
				}
			}
		}

		return 0;
	}
	else
	{
		Elapsedtime += delta;
		auto& camComponent = camera;
		auto& PlayerComponent = m_Registry.get<Cars>(m_PCar);
		auto& EnemyComponent = m_Registry.get<Cars>(m_enemy);
		int temp;
		glm::vec3 curCamPos = camComponent->GetPosition();
		glm::vec3 curCamFor = camComponent->GetForward();

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			curCamPos.x += 10.f * delta;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			curCamPos.x -= 10.f * delta;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			curCamPos.y -= 10.f * delta;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			curCamPos.y += 10.f * delta;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			curCamFor.x += 0.1f * delta;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			curCamFor.x -= 0.1f * delta;
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			curCamFor.y -= 0.1f * delta;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			curCamFor.y += 0.1f * delta;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			PlayerComponent.Draw();
		}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
		{
			escRelease = true;
		}
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && escRelease)
		{
			isPaused = true;
			escRelease = false;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && Elapsedtime >= 0.5)
		{
			double* x = new double;
			double* y = new double;

			glfwGetCursorPos(window, x, y);
			if (*x >= 70 && *x <= 95 && *y <= 615 && *y >= 597)
			{
				lbutton_down = true;
			}
			//printf("Mouse at X %f Y %f\n", *x, *y);
			if (m_Registry.get<syre::PathAnimator>(m_PCar).HitMax() || m_Registry.get<syre::PathAnimator>(m_enemy).HitMax())
			{
				for (float i = 0; i <= 5; i++)
				{
					if ((i * 165) + 429 <= *x && (i + 1) * 165 + 429 >= *x && *y >= 457 && *y <= 706 && PlayerComponent.GetCard(i, true) != -1)
					{
						if (PlayerComponent.GetCard(i, true) == 2)
						{
							temp = PlayerComponent.GetGear();
							PlayerComponent.PlayCard(i, EnemyComponent.GetGear());
							EnemyComponent.ChangeGears(temp);
						}
						else
						{
							PlayerComponent.PlayCard(i, 0);
						}
						Elapsedtime = 0;
					}
				}
				if (*x >= 34 && *x <= 72 && *y <= 699 && *y >= 674)
				{
					PlayerComponent.SetBrk();
					Elapsedtime = 0;
				}
				else if (*x >= 98 && *x <= 129 && *y <= 709 && *y >= 666)
				{
					PlayerComponent.SetAcc();
					Elapsedtime = 0;
				}
			}
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			lbutton_down = false;
		}
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		{
			m_Registry.get<syre::PathAnimator>(m_PCar).Reset();
		}
		camComponent->SetPosition(curCamPos);
		return 0;
	}
}

