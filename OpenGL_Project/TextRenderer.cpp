#include "TextRenderer.h"
#include "RenderingPipeline.h"
#include <iostream>

void TextRenderer::SetText(std::string _content)
{
	text = _content;
}

void TextRenderer::SetFont(std::string _fontKey)
{
    fontKey = _fontKey;
}

void TextRenderer::SetColor(glm::vec3 _color)
{
	color = _color;
}

void TextRenderer::InitVBO() 
{
    glGenBuffers(1, &VBO);

    BindVBOData();
}

void TextRenderer::BindVBOData() 
{
    glBindVertexArray(mesh->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextRenderer::Update()
{
    translationMat = glm::translate(glm::mat4(1.0f), parent->GetPosition());

    rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(parent->GetRotation().z), glm::vec3(0.f, 0.f, 1.f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(parent->GetRotation().y), glm::vec3(0.f, 1.f, 0.f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(parent->GetRotation().x), glm::vec3(1.f, 0.f, 0.f));

    scaleMat = glm::scale(glm::mat4(1.0f), parent->GetScale());

    modelMat = translationMat * rotationMat * scaleMat;

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4), &modelMat, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

TextRenderer::TextRenderer(std::string _shaderKey, ProjectionType _projectionType) : Renderer(_shaderKey, _projectionType)
{
	translationMat = glm::mat4();
	rotationMat = glm::mat4();
	scaleMat = glm::mat4();
	modelMat = glm::mat4();

	color = glm::vec3(1.f, 1.f, 1.f);

    renderShadows = false;
}

TextRenderer::~TextRenderer()
{
}

void TextRenderer::InitializeRenderingInfo(GLuint _program)
{
	glUniform3f(glGetUniformLocation(_program, "Color"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);

	glUniformMatrix4fv(glGetUniformLocation(_program, "VP"), 1, GL_FALSE, glm::value_ptr(Camera::Instance().GetProjectionMatrix(projection)));
}

void TextRenderer::Init()
{
	RenderingPipeline::AddRenderer(this);
}

void TextRenderer::Render()
{
    float totalHeight = 0;
    float totalWidth = 0;

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); ++c)
    {
        TextCharacter ch = AssetLoader::Instance().GetGlyph(fontKey, *c);

        float height = ch.size.y * parent->GetScale().y;

        if (height > totalHeight) 
        {
            totalHeight = height;
        }

        totalWidth += (ch.advanceOffset >> 6) * parent->GetScale().x;
    }

    int x = parent->GetPosition().x - (totalWidth / 2.0f);
    int y = parent->GetPosition().y - (totalHeight / 2.0f);


	glBindVertexArray(mesh->VAO);

    glDisable(GL_DEPTH_TEST);

    for (c = text.begin(); c != text.end(); ++c)
    {
        TextCharacter ch = AssetLoader::Instance().GetGlyph(fontKey, *c);

        float xpos = x + ch.bearing.x * parent->GetScale().x;
        float ypos = y - (ch.size.y - ch.bearing.y) * parent->GetScale().y;

        float width = ch.size.x * parent->GetScale().x;
        float height = ch.size.y * parent->GetScale().y;

        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + height,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + width, ypos,       1.0f, 1.0f },

            { xpos,     ypos + height,   0.0f, 0.0f },
            { xpos + width, ypos,       1.0f, 1.0f },
            { xpos + width, ypos + height,   1.0f, 0.0f }
        };

        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.advanceOffset >> 6) * parent->GetScale().x; // bitshift by 6 to get value in pixels (2^6 = 64)
    }

    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
