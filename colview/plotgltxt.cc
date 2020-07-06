
#include "colv.hh"

GLuint program;

struct Character {
  GLuint TextureID;   // ID handle of the glyph texture
  glm::ivec2 Size;    // Size of glyph
  glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
  GLuint Advance;     // Horizontal offset to advance to next glyph
};

#include <ft2build.h>
#include FT_FREETYPE_H
#include <fontconfig/fontconfig.h>

#include <glm/gtc/type_ptr.hpp>
#include <map>
std::map<GLchar, Character> Characters;
GLuint VAOt, VBO;


int SizeInPixels;


string GetFontFileName(string name) {
  cout << "name: " << name << endl;
  
  FcConfig  * cfg = FcInitLoadConfigAndFonts();
  FcPattern * pat = FcNameParse((const FcChar8*)(name.c_str()));
  FcConfigSubstitute(cfg, pat, FcMatchPattern);
  FcDefaultSubstitute(pat);

  string fontFile = "";
  FcResult result;
  FcPattern * font = FcFontMatch(cfg, pat, &result);
  
  if(font) {
    FcChar8 * file = NULL;
    if(FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch)
      fontFile = string( (char*)file);
    
    FcPatternDestroy(font);
  }
  FcPatternDestroy(pat);

  cout << "fontFile: " << fontFile << endl;
  
  return fontFile;
}



void InitTextures() {
  SizeInPixels = 96;
  

  FT_Library ft;   // FreeType
  if( FT_Init_FreeType(&ft))
    cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;

  FT_Face face;  // Load font as face
  
  string theFontFileName = GetFontFileName(string("DejaVuSansMono"));
  if(theFontFileName == string("") )
    theFontFileName = GetAbsoluteFile( string("Cantarell-Regular.otf") );

  if (FT_New_Face(ft, theFontFileName.c_str(), 0, &face))
    cout << "ERROR::FREETYPE: Failed to load font" << endl;
  else
    cout << "Font: " << theFontFileName << endl;

  // Set size to load glyphs as
  FT_Set_Pixel_Sizes(face, 0, SizeInPixels);

  // use 50pt at 100dpi
  //FT_Set_Char_Size( face, 50 * 64, 0, 100, 0 );
    
  // Disable byte-alignment restriction
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

  // Load first 128 characters of ASCII set
  for (GLubyte c = 0; c < 128; c++) {
    // Load character glyph 
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      cout << "ERROR::FREETYTPE: Failed to load Glyph" << endl;
      continue;
    }

    // cout << "ft2pixels: " << face->glyph->bitmap.rows << " x " << face->glyph->bitmap.width
    //  	 << " " << face->glyph->bitmap.num_grays << endl;
 

    // Generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D( GL_TEXTURE_2D,
		  0,
		  GL_RED,
		  face->glyph->bitmap.width,
		  face->glyph->bitmap.rows,
		  0,
		  GL_RED,
		  GL_UNSIGNED_BYTE,
		  face->glyph->bitmap.buffer
		  );

    // Set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // store character for later use
    Character character = {
      texture,
      glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
      glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
      (GLuint)face->glyph->advance.x
    };
    Characters.insert(std::pair<GLchar, Character>(c, character));
  }
  
  glBindTexture(GL_TEXTURE_2D, 0);
  // Destroy FreeType once we're finished
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

    
  // Configure VAO/VBO for texture quads
  glGenVertexArrays(1, &VAOt);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAOt);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}



void InitShadersText() {
  InitTextures();
  
  program = LoadShaders("shaderstxt.glsl");
  //glUseProgram(program);
}



void RenderText(string text, vec3 xys, vec3 plotsize, vec3 color) {
  float x = xys.x;
  float y = xys.y;
  float scale = xys.z * 1.3 / (float)SizeInPixels;
  //cout << "Scale: " << scale << " inv " << 1.0/scale << endl;

  mat4 projection = ortho(0.0f, (float)plotsize.x, 0.0f, (float)plotsize.y);
 
  glUseProgram(program);
  glUniformMatrix4fv(glGetUniformLocation(program, "projection"),
		     1, GL_FALSE, glm::value_ptr(projection));
  glUniform3f(glGetUniformLocation(program, "textColor"), color.x, color.y, color.z);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAOt);

  // Iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    Character ch = Characters[*c];

    GLfloat xpos = x + ch.Bearing.x * scale;
    GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    GLfloat w = ch.Size.x * scale;
    GLfloat h = ch.Size.y * scale;
    // Update VBO for each character
    GLfloat vertices[6][4] = { { xpos,     ypos + h,   0.0, 0.0 },            
			       { xpos,     ypos,       0.0, 1.0 },
			       { xpos + w, ypos,       1.0, 1.0 },

			       { xpos,     ypos + h,   0.0, 0.0 },
			       { xpos + w, ypos,       1.0, 1.0 },
			       { xpos + w, ypos + h,   1.0, 0.0 }           
    };
    
    // Render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.TextureID);

    // Update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);       // Render quad
    // advance cursors for next glyph (1/64 pixels)
    //x += (ch.Advance >> 6) * scale;
    x += (ch.Advance /64) * scale;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);
}
