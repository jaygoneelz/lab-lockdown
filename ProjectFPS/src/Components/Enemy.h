#pragma once

#include "Puppet.h"

class BoxCollider;
class Light;
enum class EnemyState
{
	Roaming,
	Chasing,
    Attack
};

class Enemy : public Puppet
{
public:
    // Current AI decision tree state)
    EnemyState currentState = EnemyState::Roaming;

    // Chasing behavior
    float chaseDistance = 10.0f; 
    Transform* playerTransform = nullptr; 

    //Attack Speed
    float windupRemaining = 0.0f;
    float entranceRemaining = 0.0f;
    float recoveryRemaining = 0.0f;
    float shutdownElapsed = 0.0f;
    Light* warningLight = nullptr;
    int attackDamage = 5;
    float TimeBetweenAttack = 4.0f;
    float AttackTimer = 0;

    // Movement
    float moveSpeed = 2.0f;  

    void SetPatrolPoints(const std::vector<glm::vec3>& points)
    {
        patrolPoints = points;
    }

    std::vector<glm::vec3> patrolPoints; // List of positions for patrol
    int currentPatrolIndex = 0;          // Current patrol target index
    float patrolThreshold = 0.5f;       // Distance threshold to switch to the next patrol point

    bool DeadOnce = false;
    void TakeDamage(float damage) override;
    bool CoreExposed() const { return attackDamage > 5 && recoveryRemaining > 0.0f; }

private:
	void Start() override;
	void Update(float deltaTime) override;
    void DecideState();
    void UpdatePatrolling(float deltaTime);
    void UpdateChasing(float deltaTime); 
    void UpdateAttack(float deltatime);

    float rotationAngle;
    float flinchRemaining = 0.0f;
    float baseModelTilt = 0.0f;
    void LookInMovementDirection(glm::vec3 Direction ,float deltatime);
};