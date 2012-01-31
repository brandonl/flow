#include <vector>

#include <GL\glew.h>
#include <GL\glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "Core/shader.h"
#include "Core/texture.h"

#include "text2d.h"

unsigned int Text2DVertexBufferID;
unsigned int Text2DUVBufferID;
unsigned int vao;
Shader fontShader;
Texture fontTexture;

namespace Text
{
	void init(const char * texturePath){

		// Initialize texture
		Texture t;
		t.load( texturePath, 0 );

		glGenVertexArrays( 1, &vao );
		glBindVertexArray( vao );

		// Initialize VBO
		glGenBuffers(1, &Text2DVertexBufferID);
		glGenBuffers(1, &Text2DUVBufferID);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

		glBindVertexArray( 0 );
		glBindBuffer(GL_ARRAY_BUFFER, 0 );
	
		// Initialize Shader
		fontShader.load( "../../content/Shaders/text" );
	}

	void print(const char * text, int x, int y, int size){

		unsigned int length = strlen(text);

		// Fill buffers
		std::vector<glm::vec2> vertices;
		std::vector<glm::vec2> UVs;
		for ( unsigned int i=0 ; i<length ; i++ ){
		
			glm::vec2 vertex_up_left    = glm::vec2( x+i*size     , y+size );
			glm::vec2 vertex_up_right   = glm::vec2( x+i*size+size, y+size );
			glm::vec2 vertex_down_right = glm::vec2( x+i*size+size, y      );
			glm::vec2 vertex_down_left  = glm::vec2( x+i*size     , y      );

			vertices.push_back(vertex_up_left   );
			vertices.push_back(vertex_down_left );
			vertices.push_back(vertex_up_right  );

			vertices.push_back(vertex_down_right);
			vertices.push_back(vertex_up_right);
			vertices.push_back(vertex_down_left);

			char character = text[i];
			float uv_x = (character%16)/16.0f;
			float uv_y = (character/16)/16.0f;

			glm::vec2 uv_up_left    = glm::vec2( uv_x           , 1.0f - uv_y );
			glm::vec2 uv_up_right   = glm::vec2( uv_x+1.0f/16.0f, 1.0f - uv_y );
			glm::vec2 uv_down_right = glm::vec2( uv_x+1.0f/16.0f, 1.0f - (uv_y + 1.0f/16.0f) );
			glm::vec2 uv_down_left  = glm::vec2( uv_x           , 1.0f - (uv_y + 1.0f/16.0f) );
			UVs.push_back(uv_up_left   );
			UVs.push_back(uv_down_left );
			UVs.push_back(uv_up_right  );

			UVs.push_back(uv_down_right);
			UVs.push_back(uv_up_right);
			UVs.push_back(uv_down_left);
		}

		glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
		glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);

		// Bind shader
		glUseProgram( fontShader.get_name() );

		// Bind texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fontTexture.get_name() );
		
		fontShader.set_int( "myTextureSampler", 0 );

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBindVertexArray( vao );
		// Draw call
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() );

		// Reset State
		glDisable(GL_BLEND);
		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glUseProgram( 0 );
		glBindTexture( GL_TEXTURE_2D, 0 );

	}

	void clean(){

		// Delete buffer
		glDeleteBuffers(1, &Text2DVertexBufferID);
		glDeleteBuffers( 1, &Text2DUVBufferID );

	}
};
