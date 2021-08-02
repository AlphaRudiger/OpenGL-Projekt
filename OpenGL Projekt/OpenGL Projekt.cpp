#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <crtdbg.h>

#include <string.h>
#include <iostream>
#include <string>
#include <thread>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Font.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "PerspectiveCamera.h"
#include "FirstPersonCameraController.h"
#include "Texture.h"
#include "Chunk.h"
#include "World.h"
#include "Utils.h"
#include "TextureAtlas.h"
#include "Input.h"
#include "Cubemap.h"
#include "ShapeRenderer.h"
#include "Player.h"
#include "SpriteBatch.h"
#include "Viewport.h"
#include "Raycast.h"

#include "Profiler.h"

#include <vector>

using namespace std;

typedef unsigned int uint;

GLFWwindow* window;

bool frustum = true;

void GLAPIENTRY mglErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cout << "Error: " << std::endl;
    std::cout << message << std::endl;
	ASSERT(0);
}

Profiler profiler({ {"WR", Color::RED }, {"Sort", Color(1, 1, 0) }, { "ORender", Color::BLUE }, { "Update", Color::GREEN }, { "Generate", Color(1, 0, 1) }});

void centerWindow(GLFWwindow* window, GLFWmonitor* monitor = glfwGetPrimaryMonitor()) {
	if(!monitor)
		return;

	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	if(!mode)
		return;

	int monitorX, monitorY;
	glfwGetMonitorPos(monitor, &monitorX, &monitorY);

	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	glfwSetWindowPos(window,
		monitorX + (mode->width - windowWidth) / 2,
		monitorY + (mode->height - windowHeight) / 2);
}

Player* stupid;

#include "Math.h"

int main(void) {
	_CrtDumpMemoryLeaks();
    {
		cout << sum(Vector2 { 0.2, 0.3 }) << endl;
		cout << "det: " << Matrix2({ 5, -7, -6, 4 }).det() << endl;
		cout << "dot: " << dot(Vector2({ 1, 2 }), Vector2({ 3, 4 })) << endl;
		Matrix2 mat = { 0, 2, 1, 0 };
		mat.idk();
		Matrix2 mat2 = { 1, -2, 1, 0 };
		mat2.idk();
		Vector2 v = { 1, 1 };
		Vector3 vr = cross(Vector3({ 5, -4, 3 }), { -7, 2, -8 });
		cout << "cross: " << vr << endl;;

		cout << mat << endl;
		cout << v << endl << endl;
		auto r = mat * mat2;
		cout << "result: " << r << endl;


		//return 0;
        /* Initialize the library */
        if (!glfwInit())
            return -1;

        currentWidth = 1920 * 0.9;
        currentHeight = 1080 * 0.9;

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow((int) currentWidth, (int) currentHeight, "Minecraft", NULL, NULL);
        if (!window) {
            glfwTerminate();
            return -1;
        }


        /* Make the window's context current */
        glfwMakeContextCurrent(window);

		centerWindow(window);

        glewInit();

        glfwSwapInterval(0);

        glfwSetWindowSizeCallback(window, window_size_callback);

#ifdef DEBUG
		glDebugMessageCallback(mglErrorCallback, nullptr);
#endif


        TextureAtlas atlas("assets\\textures\\Textures.png", "assets\\textures\\textures.atlas");
        loadTextures(atlas); 

        ShaderProgram sp("assets\\shader\\game\\VertexShader.glsl", "assets\\shader\\game\\FragmentShader.glsl", true);

        sp.bind();

        World world(2);
        //TestSpeed(world.generateChunk(0, 0, true));
        Player* player = new Player(&world, 0, 180, 0);
		stupid = player;
        p = player;
        world.addEntity(player);

        sp.bind();

        world.player = player;

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        Input::load(window);

        currentInputProcessor = &player->controller;

        bool end = false;

        vector<std::thread*> threads;


        for(int i = 0; i < GENERATOR_THREADS; i++) {
            threads.push_back(new std::thread([](World* world, bool* end) {
                while(!*end) {
                    shared_ptr<GeneratePromise> p;
                    p = world->getChunkSupplier().getNextChunkToGenerate();
                    if(p != nullptr) {
                        p->fulfill();
                    } else {
                        using namespace std::chrono_literals;
                        std::this_thread::sleep_for(1ms);
                    }
                }
                if(*end) cout << "END THREAD" << std::endl;
            }, &world, &end));
        }

        std::string sbi[6]{
            "assets\\textures\\Skybox\\Daylight Box_Right.bmp",
            "assets\\textures\\Skybox\\Daylight Box_Left.bmp",
            "assets\\textures\\Skybox\\Daylight Box_Top.bmp",
            "assets\\textures\\Skybox\\Daylight Box_Bottom.bmp",
            "assets\\textures\\Skybox\\Daylight Box_Front.bmp",
            "assets\\textures\\Skybox\\Daylight Box_Back.bmp",
        };
        Skybox skybox(sbi, &player->controller);

        ShapeRenderer sr;

        auto time = currentTimeMilis(); 
        SpriteBatch batch;
        cout << "Start loop" << endl;

		Font font = { "assets/el/Font.png", "assets/el/FontData.csv" };

		int fps = 0;
		int fpsc = 0;

		float tc = 0;

		float pt = 0;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)) {
			float delta = (currentTimeMilis() - time) / 1000.0f;
			pt += delta;
			tc += delta;
			fpsc++;
			if(tc >= 0.25) {
				tc = 0;
				fps = fpsc * 4;
				fpsc = 0;
			}
			delta = min(delta, 1.f / 30.f);
            time = currentTimeMilis();

			glEnable(GL_DEPTH_TEST);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //player.update(delta);

			if(GENERATOR_THREADS < 1) {

				PROFILE(profiler, "Generate", {
					shared_ptr<GeneratePromise> p = world.getChunkSupplier().getNextChunkToGenerate();
					if(p != nullptr) {
						p->fulfill();
					}
				});
			}
            //world.getChunkSupplier().monitor(delta);

            PROFILE(profiler, "Update", world.updateEntitys(delta));

            sp.bind();
            sp.setUniformMat4f("matrix", player->cam.combined);
            sp.setUniformVec3f("playerPosition", player->position.x, player->position.y, player->position.z);
            atlas.texture.bind();
			world.draw();

			PROFILE(profiler, "ORender", {
				skybox.draw();

				sr.begin(ShapeType::Line);
				sr.setTransforamtionMatrix(player->cam.combined);
				sr.setColor(Color::WHITE);
				if(frustum) sr.drawLine(0, 0, 0, 0, 1000, 0);

				glm::vec3 lm = player->cam.position + player->cam.direction;

				Block* b = player->lookingAt;
				if(b != nullptr) {
					const float d = 0.002f;
					sr.setColor(Color::BLACK);
					sr.box(b->getAbsoluteX() - d, b->getAbsoluteY() - d, b->getAbsoluteZ() - d, 1 + d * 2, 1 + d * 2, 1 + d * 2);
				}

				sr.end();

				glDisable(GL_DEPTH_TEST);

				glLineWidth(2);
				sr.begin(ShapeType::Line);
				sr.setColor(Color::BLUE);
				sr.drawLine(lm.x, lm.y, lm.z, lm.x, lm.y, lm.z + 0.1f);
				sr.setColor(Color::RED);
				sr.drawLine(lm.x, lm.y, lm.z, lm.x + 0.1f, lm.y, lm.z);
				sr.setColor(Color::GREEN);
				sr.drawLine(lm.x, lm.y, lm.z, lm.x, lm.y + 0.1f, lm.z);
				sr.end();

				glEnable(GL_BLEND);
				batch.begin();
				batch.setTransformationMatrix(glm::ortho(0.f, 1920.f, 0.f, 1080.f));
				batch.setColor(0, 0, 0, 1);
				font.print(batch, string("Fps: ").append(std::to_string(fps)), { 0, currentHeight - font.getHeight() });
				if(player->controller.look) {
					Block* look = player->controller.look;
					int light = look->getBlock(look->x, look->y + 1, look->z)->light.getIntensity();
					font.print(batch, string("Top: ").append(std::to_string(light)), { 0, currentHeight - font.getHeight() * 2 });
				}
				batch.end();
				
				glLineWidth(1);
				glEnable(GL_DEPTH_TEST);
				sr.begin(ShapeType::Line);
				sr.setColor(Color::WHITE);
				sr.setTransforamtionMatrix(player->cam.combined);
				world.getChunkSupplier().forEach([](Chunk* chunk, ShapeRenderer* sr) {
					BoundingBox box = chunk->boundingBox;
					Vector3G pos = box.low;
					sr->box(pos.x, pos.y, pos.z, box.getWidth(), box.getHeight(), box.getDepth());
					}, & sr);
				sr.end();
			});

			

			glDisable(GL_DEPTH_TEST);
			sr.begin(ShapeType::Fill);
			sr.setTransforamtionMatrix(glm::ortho(0.f, 1920.f, 0.f, 1080.f));

			profiler.drawResult(sr, 100, 100, 100);

			sr.end();

			

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();

            if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
                frustum = !frustum;
            }

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                glfwSetWindowShouldClose(window, true);
            }
            
        }
        end = true;

        for (int i = 0; i < GENERATOR_THREADS; i++) {
            std::thread* thread = threads[i];
            if (thread->joinable()) thread->join();
            delete thread;
        }
    }
    glfwTerminate();

#ifdef DEBUG
    //_CrtDumpMemoryLeaks();
#endif
    return 0;
}