#ifdef COMPILING_VS
	layout (location = 0) in vec3 Position;
	layout (location = 1) in vec2 TexCoords;
	layout (location = 2) in vec3 Normal;
    layout (location = 3) in mat4 ModelMatrix;

	uniform mat4 VP;
    uniform mat4 LightVP;

	out vec2 FragTexCoords;
	out vec3 FragNormal;
	out vec3 FragPos;
    out vec4 FragPosLightSpace;

	void main() 
	{
		FragTexCoords = TexCoords;
		FragNormal = mat3(transpose(inverse(ModelMatrix))) * Normal;
		FragPos = vec3(ModelMatrix * vec4(Position, 1.0f));
        FragPosLightSpace = LightVP * vec4(FragPos, 1.0);

        gl_Position = VP * ModelMatrix * vec4(Position, 1.0f);
	}

#elif defined(COMPILING_FS)

    //IN / OUTS
    in vec2 FragTexCoords;
    in vec3 FragNormal;
    in vec3 FragPos;
    in vec4 FragPosLightSpace;

    out vec4 FinalColor;

    //BASIC
    uniform sampler2D Texture0;
    uniform float Time;


    vec3 vp = vec3(3.0, 0, 0);
    vec3 vpdir = vec3(-3., 0, 0);

    vec4 getc(float z) 
    {
        float dpc = max(min(z,1.),-1.)/2.+0.5;
        //return vec4(dpc,1.-dpc,0.,1.);
        return vec4(vec3(1.-dpc-.3),1.);
    }

    float getdz(vec2 inp) 
    {
        float x = inp.x;
        float y = inp.y;
        return -cos(y-x)-sin(x+y);
    }

    float getz(vec2 inp) 
    {
        float x = inp.x;
        float y = inp.y;
        return -sin(y-x)+cos(x+y);
        //return -4.*sin(y*y-x)+cos(x+y);
    }

    float gettdz(float t, vec2 dir) 
    {
        return vpdir.z-getdz(dir.xy * t);
    }

    float gettz(float t, vec2 dir) 
    {
        return vpdir.z*t+vp.z-getz(dir.xy * t);
    }


    void main() 
    {
        vec2 uv = 7.5 * (FragTexCoords.xy - vec2(0.5f, 0.5f));// - 0.5;
    
        float t = 0.;
        float cz = vp.z;
        //float roty = normalize(vpdir.z);
        //vec2 origin = vp-vec3(0.,uv.x,uv.y);
    
        vpdir = vec3(0.) -vp -vec3(normalize(vpdir.xy)* uv.x, uv.y);
    
        vec2 dir = normalize(vpdir.xy + uv + Time / 1000.);
    
        //calculate zero using newton's method
        float pv = 0., cv = 0.;
        //x_(n + 1) = x_n - (x_n cos(x_n))/(cos(x_n) - x_n sin(x_n))
        for(int i = 0; i < 20; i++) {
            pv = cv;
            cv = cv - gettz(cv,dir)/gettdz(cv,dir);
            //if((abs(pv-cv)>0.01) break;
        }
    
	    //fragColor = vec4(uv,0.5+0.5*sin(iTime),1.0);
        float z = getz(dir * pv);//.05*
        FinalColor = texture(Texture0, FragTexCoords * (1.0f - min(0.5f * z, 0.2f)));
    }

#endif