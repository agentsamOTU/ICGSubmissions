#include "MidtermScene.h"

MidtermScene::MidtermScene(GLFWwindow* inWind)
{
	window = inWind;
}

void MidtermScene::Start()
{
	//framebuffer and effect setup
	int width, height;
	glfwGetWindowSize(window, &width, &height);

	sceneBuff = m_Registry.create();
	combinedBuf = m_Registry.create();
	gBuff = m_Registry.create();
	deferredBuff = m_Registry.create();

	m_Registry.emplace<PostEffect>(sceneBuff);
	m_Registry.emplace<CombinedBloom>(combinedBuf);
	m_Registry.emplace<GBuffer>(gBuff);
	m_Registry.emplace<DeferredLighting>(deferredBuff);

	m_Registry.get<PostEffect>(sceneBuff).Init(width, height);
	m_Registry.get<CombinedBloom>(combinedBuf).Init(width, height);
	m_Registry.get<GBuffer>(gBuff).Init(width,height);
	m_Registry.get<DeferredLighting>(deferredBuff).Init(width, height);


	//camera setup
	camera = Camera::Create();

	camera->SetPosition(glm::vec3(-6, 0, 0)); // Set initial position
	camera->SetUp(glm::vec3(0, 0, 1)); // Use a z-up coordinate system
	camera->LookAt(glm::vec3(0.0f)); // Look at center of the screen
	camera->SetFovDegrees(100.0f); // Set an initial FOV

	//shader setup
	basicShader = Shader::Create();
	basicShader->LoadShaderPartFromFile("vertex_shader.glsl", GL_VERTEX_SHADER);
	basicShader->LoadShaderPartFromFile("frag_shader.glsl", GL_FRAGMENT_SHADER);
	basicShader->Link();

	toonShader = Shader::Create();
	toonShader->LoadShaderPartFromFile("vertex_shader.glsl", GL_VERTEX_SHADER);
	toonShader->LoadShaderPartFromFile("toonFrag_shader.glsl", GL_FRAGMENT_SHADER);
	toonShader->Link();

	gBuffShader = Shader::Create();
	gBuffShader->LoadShaderPartFromFile("vertex_shader.glsl", GL_VERTEX_SHADER);
	gBuffShader->LoadShaderPartFromFile("gFrag_shader.glsl", GL_FRAGMENT_SHADER);
	gBuffShader->Link();

	glm::vec3 lightPos = glm::vec3(-3.0f, 0.0f, -2.0f);
	glm::vec3 lightCol = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 ambientCol = glm::vec3(1.0f);

	basicShader->SetUniform("u_LightPos", lightPos);
	basicShader->SetUniform("u_LightCol", lightCol);
	basicShader->SetUniform("u_AmbientCol", ambientCol);

	toonShader->SetUniform("u_LightPos", lightPos);
	toonShader->SetUniform("u_LightCol", lightCol);
	toonShader->SetUniform("u_AmbientCol", ambientCol);

	//object setup
	goldenMonkey = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(goldenMonkey, "monkey_quads.obj");
	m_Registry.emplace<syre::Texture>(goldenMonkey, "goldenBright.png");
	m_Registry.emplace<syre::Transform>(goldenMonkey, glm::vec3(0.0f));

	entt::entity pedestal = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(pedestal, "pedestal.obj");
	m_Registry.emplace<syre::Texture>(pedestal, "stone-1024.png");
	m_Registry.emplace<syre::Transform>(pedestal, glm::vec3(0.0f,0.0f,-3.5f),glm::vec3(90.0f,0.f,0.0f),glm::vec3(0.4f));

	entt::entity pedestal1 = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(pedestal1, "pedestal.obj");
	m_Registry.emplace<syre::Texture>(pedestal1, "stone-1024.png");
	m_Registry.emplace<syre::Transform>(pedestal1, glm::vec3(0.0f, -4.0f, -3.3f), glm::vec3(175.0f, 0.f, 0.0f), glm::vec3(0.4f));

	entt::entity pedestal2 = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(pedestal2, "pedestal.obj");
	m_Registry.emplace<syre::Texture>(pedestal2, "stone-1024.png");
	m_Registry.emplace<syre::Transform>(pedestal2, glm::vec3(0.0f, 4.0f, -3.3f), glm::vec3(5.0f, 0.f, 0.0f), glm::vec3(0.4f));

	entt::entity circle = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(circle, "circlePlane.obj");
	m_Registry.emplace<syre::Texture>(circle, "dirtDark.png");
	m_Registry.emplace<syre::Transform>(circle, glm::vec3(0.0f, 0.0f, -4.0f), glm::vec3(90.0f, 0.f, 0.0f), glm::vec3(10.0f));

	entt::entity neonLight1 = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(neonLight1, "neonLightReceptacle.obj");
	m_Registry.emplace<syre::Texture>(neonLight1, "superBrightGreen.png");
	m_Registry.emplace<syre::Transform>(neonLight1, glm::vec3(0.0f, -2.0f, -3.9f), glm::vec3(90.0f, 0.f, 0.0f), glm::vec3(0.1f));

	entt::entity neonLight2 = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(neonLight2, "neonLightReceptacle.obj");
	m_Registry.emplace<syre::Texture>(neonLight2, "superBrightGreen.png");
	m_Registry.emplace<syre::Transform>(neonLight2, glm::vec3(0.0f, 2.0f, -3.9f), glm::vec3(90.0f, 0.f, 0.0f), glm::vec3(0.1f));

	entt::entity neonLight3 = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(neonLight3, "neonLightReceptacle.obj");
	m_Registry.emplace<syre::Texture>(neonLight3, "superBrightGreen.png");
	m_Registry.emplace<syre::Transform>(neonLight3, glm::vec3(2.0f, 0.0f, -3.9f), glm::vec3(90.0f, 0.f, 90.0f), glm::vec3(0.1f));

	entt::entity neonLight4 = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(neonLight4, "neonLightReceptacle.obj");
	m_Registry.emplace<syre::Texture>(neonLight4, "superBrightGreen.png");
	m_Registry.emplace<syre::Transform>(neonLight4, glm::vec3(-2.0f, 0.0f, -3.9f), glm::vec3(90.0f, 0.f, 90.0f), glm::vec3(0.1f));

	entt::entity neonLight5 = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(neonLight5, "neonLightReceptacle.obj");
	m_Registry.emplace<syre::Texture>(neonLight5, "superBrightRed.png");
	m_Registry.emplace<syre::Transform>(neonLight5, glm::vec3(-2.0f, -4.0f, -3.9f), glm::vec3(90.0f, 0.f, 90.0f), glm::vec3(0.1f));

	entt::entity neonLight6 = m_Registry.create();
	m_Registry.emplace<syre::Mesh>(neonLight6, "neonLightReceptacle.obj");
	m_Registry.emplace<syre::Texture>(neonLight6, "superBrightRed.png");
	m_Registry.emplace<syre::Transform>(neonLight6, glm::vec3(-2.0f, 4.0f, -3.9f), glm::vec3(90.0f, 0.f, 90.0f), glm::vec3(0.1f));

	lastFrame = glfwGetTime();

}

int MidtermScene::Update()
{
	PostEffect* framebuffer = &m_Registry.get<PostEffect>(sceneBuff);
	GBuffer* gFramebuffer = &m_Registry.get<GBuffer>(gBuff);
	CombinedBloom* bloom = &m_Registry.get<CombinedBloom>(combinedBuf);
	DeferredLighting* deferred = &m_Registry.get<DeferredLighting>(deferredBuff);

	framebuffer->Clear();
	gFramebuffer->Clear();
	bloom->Clear();
	deferred->Clear();

	thisFrame = glfwGetTime();
	float deltaTime = thisFrame - lastFrame;
	if (deferredOn)
	{
		gFramebuffer->BindBuffer(0);

	}
	else
	{
		framebuffer->BindBuffer(0);
	}
	/*if (toonOn)
	{
		activeShader = toonShader;
	}
	*/
	if (deferredOn)
	{
		activeShader = gBuffShader;
	}
	else
	{
		activeShader = basicShader;
	}


	activeShader->Bind();
	activeShader->SetUniform("u_CamPos", camera->GetPosition());
	activeShader->SetUniform("u_SpecularStrength", specularOn ? 1.0f : 0.0f);
	activeShader->SetUniform("u_AmbientStrength", ambientOn ? 0.6f : 0.0f);
	activeShader->SetUniform("u_DiffuseStrength", diffuseOn ? 1.0f : 0.0f);
	activeShader->SetUniform("u_TextureOn", textureOn ? 1 : 0);
	activeShader->SetUniform("u_toonBands",toonOn? toonBands:0);
	
	glm::vec3 monkRot = m_Registry.get<syre::Transform>(goldenMonkey).GetRotation();
	glm::vec3 monkPos = m_Registry.get<syre::Transform>(goldenMonkey).GetPosition();


	m_Registry.get<syre::Transform>(goldenMonkey).SetRotation(glm::vec3(monkRot.x, monkRot.y, monkRot.z + deltaTime*100));
	m_Registry.get<syre::Transform>(goldenMonkey).SetPosition(glm::vec3(monkPos.x, monkPos.y, 0.5f+sin(glfwGetTime())/3));

	auto renderView = m_Registry.view<syre::Mesh, syre::Transform, syre::Texture>();
	for (auto entity : renderView)
	{
		glm::mat4 transform = renderView.get<syre::Transform>(entity).GetModelMat();
		activeShader->SetUniformMatrix("u_ModelViewProjection", camera->GetViewProjection() * transform);
		activeShader->SetUniformMatrix("u_Model", transform);
		activeShader->SetUniformMatrix("u_ModelRotation", glm::transpose(glm::inverse(glm::mat3(transform))));
		renderView.get<syre::Texture>(entity).Bind();
		renderView.get<syre::Mesh>(entity).Render();
	}
	
	if (deferredOn)
	{
		gFramebuffer->UnBindBuffer();
		deferred->UpdateUniforms(camera->GetPosition(), specularOn, ambientOn, diffuseOn, textureOn, toonOn, toonBands);
		deferred->ApplyEffect(gFramebuffer);
		framebuffer = deferred;
	}
	else
	{
		framebuffer->UnBindBuffer();
	}
	
	if (bloomOn)
	{

		bloom->ApplyEffect(framebuffer);

		bloom->DrawToScreen();
	}
	else
	{
		if (dispGbuffer)
		{
			gFramebuffer->DrawToScreen(gBuffTarg);
		}
		else
		{
			framebuffer->DrawToScreen();
		}
	}

	camera->SetForward(glm::normalize(m_Registry.get<syre::Transform>(goldenMonkey).GetPosition() - camera->GetPosition()));

	lastFrame = thisFrame;

	return 0;
}

void MidtermScene::ImGUIUpdate()
{
	// Implementation new frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	// ImGui context new frame
	ImGui::NewFrame();
	CombinedBloom* bloom = &m_Registry.get<CombinedBloom>(combinedBuf);
	float brightHold = bloom->GetThreshold();
	int passes = bloom->GetPasses();


	if (ImGui::Begin("Debug")) {
		// Render our GUI stuff

		glm::vec3 camPos = camera->GetPosition();
		ImGui::Checkbox("Deferred Lighting", &deferredOn);

		ImGui::SliderFloat3("Camera Position", &camPos.x, -20.f, 20.f);
		ImGui::Checkbox("Display G Buffer Targets", &dispGbuffer);
		if (dispGbuffer)
		{
			bloomOn = false;
			deferredOn = true;
			ImGui::SliderInt("Color Target", &gBuffTarg, 0, 2);
			ImGui::Text("0 is color, 1 is position, 2 is normals");
		}
		ImGui::Checkbox("Ambient Lighting", &ambientOn);
		ImGui::Checkbox("Diffuse Lighting", &diffuseOn);
		ImGui::Checkbox("Specular Lighting", &specularOn);
		ImGui::Checkbox("Bloom Effect", &bloomOn);
		if (bloomOn)
		{
			ImGui::SliderFloat("Bright threshold", &brightHold, 0.0, 1.0);
			ImGui::SliderInt("Blur Passes", &passes, 0, 100);
		}
		ImGui::Checkbox("Toon shading", &toonOn);
		if (toonOn)
		{
			ImGui::SliderInt("Toon bands", &toonBands, 1, 20);
		}

		bloom->SetThreshold(brightHold);
		bloom->SetPasses(passes);
		camera->SetPosition(camPos);


		if (ImGui::Button("1"))
		{
			ambientOn = false;
			diffuseOn = false;
			specularOn = false;
			bloomOn = false;
		}
		if (ImGui::Button("2"))
		{
			ambientOn = true;
			diffuseOn = false;
			specularOn = false;
			bloomOn = false;
		}
		if (ImGui::Button("3"))
		{
			ambientOn = false;
			diffuseOn = false;
			specularOn = true;
			bloomOn = false;
		}
		if (ImGui::Button("4"))
		{
			ambientOn = true;
			diffuseOn = true;
			specularOn = true;
			bloomOn = false;
		}
		if (ImGui::Button("5"))
		{
			ambientOn = true;
			diffuseOn = true;
			specularOn = true;
			bloomOn = true;
		}
		if (ImGui::Button("6"))
		{
			textureOn = !textureOn;
		}
		
	}
	ImGui::End();

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

Camera::sptr& MidtermScene::GetCam()
{
	// TODO: insert return statement here
	return camera;
}
