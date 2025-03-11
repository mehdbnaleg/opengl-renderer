#version 330 core
struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
};

struct DirLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform DirLight dirLight;
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

struct PointLight {
  vec3 position;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
#define NR_POINT_LIGHTS 1
uniform PointLight pointLights[NR_POINT_LIGHTS];
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

struct SpotLight {
  vec3 position;
  vec3 direction;
  float innerCutoff;
  float outerCutoff;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform SpotLight spotLight;
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform Material material;
uniform vec3 viewPos;

void main() {
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);

  // vec3 result = CalcDirLight(dirLight, norm, viewDir);
  vec3 result = vec3(0.0);
  for (int i = 0; i < NR_POINT_LIGHTS; i++)
    result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
  // result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

  FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(-light.direction);

  float diff = max(dot(normal, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 225.0);

  vec3 ambient =
      light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 diffuse = light.diffuse * diff *
                 vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 specular = light.specular * spec *
                  vec3(texture(material.texture_specular1, TexCoords));

  return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 ray = light.position - FragPos;
  vec3 lightDir = normalize(ray);

  float dist = length(ray);
  float attenuation = 1.0 / (light.constant + light.linear * dist +
                             light.quadratic * (dist * dist));

  float diff = max(dot(normal, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 225.0);

  vec3 ambient =
      light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 diffuse = light.diffuse * diff *
                 vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 specular = light.specular * spec *
                  vec3(texture(material.texture_specular1, TexCoords));

  return attenuation * (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 ray = light.position - FragPos;
  vec3 lightDir = normalize(ray);

  float theta = dot(-lightDir, normalize(light.direction));
  float epsilon = light.innerCutoff - light.outerCutoff;
  float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

  float dist = length(ray);
  float attenuation = 1.0 / (light.constant + light.linear * dist +
                             light.quadratic * dist * dist);

  float diff = max(dot(normal, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 225.0);

  vec3 ambient =
      light.ambient * vec3(texture(material.texture_diffuse1, TexCoords));
  vec3 diffuse = light.diffuse * diff *
                 vec3(texture(material.texture_diffuse1, TexCoords));

  vec3 specular = light.specular * spec *
                  vec3(texture(material.texture_specular1, TexCoords));

  return attenuation * (ambient + intensity * (diffuse + specular));
}
