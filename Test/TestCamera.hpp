#ifndef TEST_CAMERA_H
#define TEST_CAMERA_H

#include "gtest/gtest.h"  // Assuming you are using Google Test
#include "Camera.h"

class TestCamera : public ::testing::Test {
protected:
    Camera camera;

    // Helper function to reset camera to default values before each test
    void SetUp() override {
        camera = Camera();
    }
};

TEST_F(TestCamera, Initialization) {
    // Test default values
    EXPECT_EQ(camera.position, glm::vec3(0.0f, 0.0f, 0.0f));
    EXPECT_EQ(camera.yaw, Camera::YAW);
    EXPECT_EQ(camera.pitch, Camera::PITCH);
    // Add more expectations for other default values
}

TEST_F(TestCamera, Movement) {
    // Test movement
    camera.processKeyboard(FORWARD, 1.0f);  // Move forward for 1 second
    EXPECT_NE(camera.position, glm::vec3(0.0f, 0.0f, 0.0f));  // Check if position changed
    // Add more expectations based on your movement logic
}

TEST_F(TestCamera, MouseMovement) {
    // Test mouse movement
    camera.processMouse(10.0f, -5.0f);
    EXPECT_NE(camera.yaw, Camera::YAW);
    EXPECT_NE(camera.pitch, Camera::PITCH);
    // Add more expectations based on your mouse movement logic
}

// Add more test functions for other test cases (Player Interaction, Accelerate, View Matrix, Keyboard Process, Scroll Process, Sensitivity)

#endif
