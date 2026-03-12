/*
 * Copyright (c) 2026 MidnightHammer-code
 * This source code is licensed under the GPL 3.0 license
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HAMMERENGINE_H
#define HAMMERENGINE_H

#include <chrono>
#include <glm/ext/vector_float3.hpp>
#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include <array>
#include <GLFW/glfw3.h>

#include <cstdint>
#include <vulkan/vulkan_core.h>



struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
};

struct MeshPushConstants {
    glm::vec3 position;
    // Note: GLM/Vulkan alignment rules usually require padding 
    // if we add more variables here later.
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete();
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};


class HammerEngine;
class HammerPipeline;

class HammerMesh {
public:
    HammerMesh(HammerEngine& engine, HammerPipeline* pipeline, 
               const std::vector<Vertex>& vertices, 
               const std::vector<uint32_t>& indices,
               glm::vec3 initialPos = glm::vec3(0.0f));
    
    ~HammerMesh();

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

    std::vector<Vertex> vertexData;

    std::vector<uint32_t> indexData;

    // Disable copying to prevent double-free of Vulkan resources
    HammerMesh(const HammerMesh&) = delete;
    HammerMesh& operator=(const HammerMesh&) = delete;

    void bindAndDraw(VkCommandBuffer commandBuffer);
    
    HammerPipeline* getPipeline() const { return pipeline; }

    void createVertexBuffer(const std::vector<Vertex>& vertices);
    void createIndexBuffer(const std::vector<uint32_t>& indices);

    void updateBuffers(std::vector<Vertex> vertexData, std::vector<uint32_t> indexData);

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;

    HammerEngine& engine;
    HammerPipeline* pipeline;

    VkBuffer vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
    VkBuffer indexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory indexBufferMemory = VK_NULL_HANDLE;

    uint32_t indexCount;
};

class HammerPipeline {
public:
    HammerPipeline(
        HammerEngine& engine, 
        std::string& vertPath, 
        std::string& fragPath,
        int renderTriangleMod,
        bool triangleRender2SideMode);

    ~HammerPipeline();

    // Disable copying to avoid double-deletion of Vulkan handles
    HammerPipeline(const HammerPipeline&) = delete;
    HammerPipeline& operator=(const HammerPipeline&) = delete;

    void bind(VkCommandBuffer commandBuffer);
    void createGraphicsPipeline(
        std::string& vertPath, 
        std::string& fragPath,
        int renderTriangleMod,
        bool triangleRender2SideMode);

    HammerEngine& hammerEngine;
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout; 
};


class HammerEngine {
public:

    uint32_t WindowWidth;
    uint32_t WindowHeight;

    bool enableValidationLayers;

    void addMeshRenderer(HammerMesh mesh);

    float yaw = -90.0f; // Horizontal angle
    float pitch = 0.0f; // Vertical angle
    float cameraSpeed = 0.01f; // Speed of movement
    float sensitivity = 0.05f; 
    bool mouse_capture = true;

    glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f); // Initial position
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);   // Forward direction
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);       // Up direction

    const char *texturePath; // path to texture atlas

    std::vector<std::unique_ptr<HammerMesh>> meshs;

    GLFWwindow* window; // GLFW window

    std::string windowName = "Hammer Engine";

    void runTest();

    int mouseLock = 1;

    float renderDistance = 512.0f; // how fare the camera can see, bigger the numbre bigger the buffers more memory needed

    float cameraFallSpeed = 0.01; // how fast the camera falls in gravaty enabled.

    void setMaxVertciesIndicesSize(VkDeviceSize maxsize); // giving the maximume size that the vertcies and incices can go to.

    void cleanup(); // engine clean up.

    void drawPassStart();

    void drawPassEnd();

    void initWindow(); // GLFW window init

    void initVulkan(); // vulkan init

    void updateCameraDefaultGravety3D(bool fall);

    void updateCameraDefault3D();

    void updateCameraDefault2D();

    void updateFrameTimeEnd();

    void drawFrame(); // function to call a new frame

    void updateFrameTimeStart();

    double currentTime = glfwGetTime();
    std::chrono::time_point<std::chrono::high_resolution_clock> start;

	double mouseX;
	double mouseY;

	float lastX = 0, lastY = 0;
	bool firstMouse = true;

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkRenderPass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	VkCommandPool commandPool;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	VkImageView textureImageView;
	VkSampler textureSampler;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;







    VkDeviceSize vertexBufferSize;
    VkDeviceSize indexBufferSize;

    VkBuffer stagingBuffer = VK_NULL_HANDLE;
    VkDeviceMemory stagingBufferMemory = VK_NULL_HANDLE;
    VkDeviceSize stagingBufferSize = 0;

    VkBuffer stagingBuffer2 = VK_NULL_HANDLE;
    VkDeviceMemory stagingBuffer2Memory = VK_NULL_HANDLE;
    VkDeviceSize stagingBuffer2Size = 0;

    std::vector<VkFence> imagesInFlight;







	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	uint32_t currentFrame = 0;



    double previousTime = glfwGetTime();
    int frameCount = 0;

    std::chrono::milliseconds totalFrameTime;

	bool framebufferResized = false;

    void createStagingBuffer();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    void mouseCallback(double xpos, double ypos);

    void mainLoopTest();

    void cleanupSwapChain();

    void recreateSwapChain();

    void createInstance();

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    void setupDebugMessenger();

    void createSurface();

    void pickPhysicalDevice();

    void createLogicalDevice();

    void createSwapChain();

    void createImageViews();

    void createRenderPass();

    void createDescriptorSetLayout();

    void createGraphicsPipeline();

    void createFramebuffers();

    void createCommandPool();

    void createDepthResources();

    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

    VkFormat findDepthFormat();

    bool hasStencilComponent(VkFormat format);

    void createTextureImage();

    void createTextureImageView();

    void createTextureSampler();

    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

    void createVertexBuffer();

    void createIndexBuffer();

    void createUniformBuffers();

    void createDescriptorPool();

    void createDescriptorSets();

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    void createCommandBuffers();

    void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

    void createSyncObjects();

    void updateUniformBuffer(uint32_t currentImage);

    VkShaderModule createShaderModule(const std::vector<char>& code);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

    bool isDeviceSuitable(VkPhysicalDevice device);

    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

    std::vector<const char*> getRequiredExtensions();

    bool checkValidationLayerSupport();

    std::vector<char> readFile(std::string& filename);

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
    
};

#endif
