//Handles the opening of the window
#include "pch.h"
#include "OpenGLWindow.h"

#include "Debug.h"
#include "Input.h"

OpenGLWindow::OpenGLWindow(unsigned int width, unsigned int height)
{
	window = nullptr;

	this->width = width;
	this->height = height;
}

OpenGLWindow::~OpenGLWindow()
{
	if (window)
		glfwDestroyWindow(window);
	glfwTerminate();
}

bool OpenGLWindow::Initialize()
{
	// Initializing GLFW
	if (!glfwInit())
	{
		Debug::LogError("GLFW", "Initialization failed!");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(width, height, "Lab Lockdown", NULL, NULL);

	if (window == NULL)
	{
		Debug::LogError("GLFW", "Window creation failed!");
		return false;
	}

	int framebufferWidth = 0;
	int framebufferHeight = 0;
	glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
	width = framebufferWidth;
	height = framebufferHeight;
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		Debug::LogError("GLEW", "Initialization failed!");
		return false;
	}

	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	SetInputCallbacks();

	glfwSetWindowUserPointer(window, this);
	return true;
}

void OpenGLWindow::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void OpenGLWindow::SetInputCallbacks()
{
	glfwSetKeyCallback(window, Input::HandleKeys);
	glfwSetCursorPosCallback(window, Input::HandleMouse);
	glfwSetMouseButtonCallback(window, Input::HandleMouseButtons);
}

void OpenGLWindow::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	OpenGLWindow* glWindow = static_cast<OpenGLWindow*>(glfwGetWindowUserPointer(window));

	glWindow->width = width;
	glWindow->height = height;
	glViewport(0, 0, width, height);
}
