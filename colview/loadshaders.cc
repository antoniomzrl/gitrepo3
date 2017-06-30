#include "colv.hh"
extern float verboseFlag, glslVersionFlag;

string ErrorMsg = "";


void ShowLinkError(GLuint programID) {
  cout << "eeeee1 ShowLinkError " << programID << endl;
  int length;
  glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
  //vector<char> buffer(length+1);
  char * buffer = (char*) g_malloc(length + 1);
  glGetProgramInfoLog(programID, length, NULL, buffer);

  ErrorMsg = ErrorMsg + "Link error:\n" + string(buffer) + "\n";
}


void ShowCompileError(GLuint shaderID, char * code) {
  cout << "eeeee2 ShowCompileError " << shaderID << endl;

  int length;
  glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
  //vector<char> buffer(length);
  char * buffer = (char*) g_malloc(length + 1);
  glGetShaderInfoLog(shaderID, length, NULL, buffer);

  ErrorMsg = ErrorMsg + "Compile error:\n" + string(buffer) + "\n"; //+ code + "\n";

  // int t, nline;
  // sscanf(&buffer[0], "%d:%d(", &t, &nline);

  // int a=0, b=0, l=0;
  // for(int i=0; l<nline; i++) {
  //   if(code[i] == '\n') {
  //     l++;
  //     a=b;
  //     b=i;
  //   }
  // }

  // code[b] = '\0';
  // printf("(line %d): %s\n", nline, &code[a+1]);
}



GLuint CompileShader(const char * Fn, float glslVersion, GLenum shaderType) {
  string code, st;

  // if( glslVersion < 3.29) code = "#version 130\n      #define GLSL130\n";
  // else                    code = "#version 330 core\n #define GLSL330\n";
  // cout << "version: " << glslVersion << code << endl;
  
  code = "#version 330 core\n #define GLSL330\n";
  
  if( shaderType == GL_FRAGMENT_SHADER) {
    code += "#define FRAGMENTSHADER\n";
    st = "FRAGMENT";
  }
  else if( shaderType == GL_VERTEX_SHADER) {
    code += "#define VERTEXSHADER\n";
    st = "VERTEX  ";
  }

  ifstream strm(Fn, ios::in);
  if( !strm.is_open() ) {
    cout << "cannot open " << Fn << endl;
    exit(-1);
  }

  cout << "Compiling " << st << " shader: " << Fn << endl;
  
  string line = "";
  while(getline(strm, line))
    code += line + "\n";
  strm.close();

  if( verboseFlag > 9 ) 
  cout << "-------shader begin:" << shaderType << endl
       << code << endl
       << "-------shader end:" << endl;
  
  GLuint shaderID = glCreateShader(shaderType);

  char const * ptr = code.c_str();
  glShaderSource(shaderID, 1, &ptr , NULL);
  glCompileShader(shaderID);

  GLint status = GL_FALSE;
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
  if(status == GL_FALSE) {
    ShowCompileError(shaderID, (char*)code.c_str() );
  }
  
  return shaderID;
}



GLuint LoadShaders(const char * shfn) {

  double T1 = myclock();
  
  float glslVersion;
  istringstream ss( (char*) glGetString(GL_SHADING_LANGUAGE_VERSION) );
  ss >> glslVersion;

  printDbg(3, "LoadShaders\n", "RED");
  cout << ss.str() << " (" << glslVersion << ")" << endl;

  if( glslVersionFlag > 0 ) glslVersion = glslVersionFlag;


  string shaderFn = GetAbsoluteFile( string(shfn) );
  GLuint vertexID   = CompileShader(shaderFn.c_str(), glslVersion, GL_VERTEX_SHADER);
  GLuint fragmentID = CompileShader(shaderFn.c_str(), glslVersion, GL_FRAGMENT_SHADER);

  cout << "Linking shaders : " << shaderFn << endl;
  GLuint programID = glCreateProgram();
  glAttachShader(programID, vertexID);
  glAttachShader(programID, fragmentID);
  glLinkProgram(programID);

  GLint status = GL_FALSE;
  glGetProgramiv(programID, GL_LINK_STATUS, &status);
  if( status == GL_FALSE ) {
    ShowLinkError(programID);
  }
  
  glDetachShader(programID, vertexID);
  glDetachShader(programID, fragmentID);
	
  glDeleteShader(vertexID);
  glDeleteShader(fragmentID);

  cout << "loadshaders : (" << myclock()-T1 << " sec)" << endl;

  if(ErrorMsg != "" ) {
    cout << ErrorMsg << endl;
    DisplayWindowText(ErrorMsg);
  }

  return programID;
}







// GLuint LoadShaders(string vertexCode, string fragmentCode) {
//   cout << "eeeee4 LoadShaders " << endl;
//   double T1 = myclock();
  
//   GLuint fragmentID = CompileShader(fragmentCode, GL_FRAGMENT_SHADER);
//   GLuint vertexID   = CompileShader(vertexCode,   GL_VERTEX_SHADER);

//   cout << "Linking shaders" << endl;
//   GLuint ProgramID = glCreateProgram();
//   glAttachShader(ProgramID, fragmentID);
//   glAttachShader(ProgramID, vertexID);
//   glLinkProgram(ProgramID);

//   ShowError(ProgramID);
  
//   glDetachShader(ProgramID, vertexID);
//   glDetachShader(ProgramID, fragmentID);
	
//   glDeleteShader(vertexID);
//   glDeleteShader(fragmentID);

//   cout << "loadshaders : (" << myclock()-T1 << " sec)" << endl;
//   return ProgramID;
// }



// GLuint CompileShader(const char * Fn, GLenum shaderType) {
//   cout << "eeeee5 CompileShader " << endl;
//   ifstream strm(Fn, ios::in);
//   if( !strm.is_open() ) {
//     cout << "cant open " << Fn << endl;
//     exit(-1);
//   }

//   printf("Compiling shader : %s\n", Fn);
    
//   string code;
//   string line = "";
//   while(getline(strm, line))
//     code += "\n" + line;
//   strm.close();

//   GLuint shaderID = CompileShader(code, shaderType);
  
//   return shaderID;
// }



// GLuint LoadShaders(const char * vertexFn ,const char * fragmentFn) {
//   cout << "eeeee6 LoadShaders " << endl;
//   double T1 = myclock();
  
//   GLuint vertexID   = CompileShader(vertexFn,   GL_VERTEX_SHADER);
//   GLuint fragmentID = CompileShader(fragmentFn, GL_FRAGMENT_SHADER);

//   cout << "Linking shaders : " << vertexFn << " " << fragmentFn << endl;
//   GLuint ProgramID = glCreateProgram();
//   glAttachShader(ProgramID, vertexID);
//   glAttachShader(ProgramID, fragmentID);
//   glLinkProgram(ProgramID);

//   GLint status = GL_FALSE;
//   glGetProgramiv(ProgramID, GL_LINK_STATUS, &status);
//   if( status == GL_FALSE ) {
//     cout << "Link error" << endl;
//     int LogLength;
//     glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &LogLength);
//     vector<char> ErrorMessage(LogLength+1);
//     glGetProgramInfoLog(ProgramID, LogLength, NULL, &ErrorMessage[0]);
//     printf("%s\n", &ErrorMessage[0]);
//   }

//   glDetachShader(ProgramID, vertexID);
//   glDetachShader(ProgramID, fragmentID);
	
//   glDeleteShader(vertexID);
//   glDeleteShader(fragmentID);

//   cout << "loadshaders : (" << myclock()-T1 << " sec)" << endl;
//   return ProgramID;
// }
