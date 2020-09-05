#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
	GLuint Program;

	//Constructor generates shader on the fly
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		//Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		//Ensure ifstream objects can throw exceptions
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);

		try
		{
			//Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			//Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			//Close File handlers
			vShaderFile.close();
			fShaderFile.close();

			//Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const GLchar* vShaderCode = vertexCode.c_str();
		const GLchar* fShaderCode = fragmentCode.c_str();

		//Compile our shader program
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];

		//Vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER); //Generates a GL_VERTEX_SHADER, returns an ID to be used
		glShaderSource(vertex, 1, &vShaderCode, NULL); //Loads the GLSL code from file to empty shader object
		glCompileShader(vertex); //Shader is then compiled

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

		//Check if shader has successfully compiled
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "VERTEX_COMPILATION_FAILED" << infoLog << std::endl;
		}

		//Fragment
		fragment = glCreateShader(GL_FRAGMENT_SHADER); //Generates a GL_FRAGMENT_SHADER, returns an ID to be used
		glShaderSource(fragment, 1, &fShaderCode, NULL); //Loads the GLSL code from file to empty shader object
		glCompileShader(fragment); //Shader is then compiled

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

		//Check if shader has successfully compiled
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "FRAGMENT_COMPILATION_FAILED" << infoLog << std::endl;
		}

		//Shader program
		this->Program = glCreateProgram(); //Creates the program and returns the ID that points to it
		glAttachShader(this->Program, vertex); //Attach both shaders to the new program
		glAttachShader(this->Program, fragment);
		glLinkProgram(this->Program); //Request the GLSL complier to ensure they are compatible

		//Check for linking errors
		glGetProgramiv(this->Program, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
			std::cout << "LINKING_FAILED" << infoLog << std::endl;
		}

		//Delete the shaders as they're linked into our program now and no longer neccessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	//Uses the current shader
	void Use()
	{
		glUseProgram(this->Program); //Loads the program onto the GPU
	}
};
#endif