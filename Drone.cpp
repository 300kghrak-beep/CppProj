#include <iostream>
#include <cstring>
#pragma region Structs
struct Sensor {
    char* name;
    float accuracy;
    float energyUsage;
};

struct Drone {
    char* droneId;
    float maxSpeed;
    Sensor* sensors;
    int sensorCount;
};

struct BaseStation {
    char* name;
    float x;
    float y;
    Drone* drones;
    int droneCount;
};

struct Network {
    char* name;
    BaseStation* stations;
    int stationCount;
};

struct System {
    Network* networks;
    int networkCount;
};
#pragma endregion
#pragma region Sensors
// if (drone.sensorCount == 0) return false;
// int idx = -1;
//
// for (int i = 0; i < drone.sensorCount; i++) {
//     if (strcmp(name, drone.sensors[i].name) == 0) {
//         idx = i;
//         break;
//     }
// }
//

Sensor createSensor(const char* name, const float accuracy,const float energyUsage) {
    Sensor emptySensor = {};
    if (!name) return emptySensor;

    Sensor sensor{};
    sensor.name = new char[strlen(name) + 1];
    strcpy(sensor.name, name);
    sensor.accuracy = accuracy;
    sensor.energyUsage = energyUsage;

    return sensor;
}

bool addSensor(Drone& drone, const Sensor& sensor) {
    if (!sensor.name) return false;
    auto* newSensors = new Sensor[drone.sensorCount + 1];

    for (int i = 0; i < drone.sensorCount; i++) {
        newSensors[i].name = new char[strlen(drone.sensors[i].name) + 1];
        strcpy(newSensors[i].name, drone.sensors[i].name);
        newSensors[i].accuracy = drone.sensors[i].accuracy;
        newSensors[i].energyUsage = drone.sensors[i].energyUsage;
    }

    newSensors[drone.sensorCount].accuracy = sensor.accuracy;
    newSensors[drone.sensorCount].energyUsage = sensor.energyUsage;
    newSensors[drone.sensorCount].name = new char[strlen(sensor.name) + 1];
    strcpy(newSensors[drone.sensorCount].name, sensor.name);

    for (int i = 0; i < drone.sensorCount; i++) {
        delete[] drone.sensors[i].name;
    }
    delete[] drone.sensors;
    drone.sensors = newSensors;
    drone.sensorCount++;

    return true;
}

// if (drone.sensorCount == 0) return false;
// int idx = -1;
//
// for (int i = 0; i < drone.sensorCount; i++) {
//     if (strcmp(name, drone.sensors[i].name) == 0) {
//         idx = i;
//         break;
//     }
// }
//


bool removeSensor(const int idx, Drone& drone) {
    if (idx < 0 || idx >= drone.sensorCount) return false;
    auto* newSensor = new Sensor[drone.sensorCount - 1];

    int newIndex = 0;
    for (int sensCount = 0; sensCount < drone.sensorCount; sensCount++) {
        if (sensCount != idx) {
            newSensor[newIndex].name = new char[strlen(drone.sensors[sensCount].name) + 1];
            strcpy(newSensor[newIndex].name, drone.sensors[sensCount].name);
            newSensor[newIndex].accuracy = drone.sensors[sensCount].accuracy;
            newSensor[newIndex].energyUsage = drone.sensors[sensCount].energyUsage;
            newIndex++;
        }
        else {
            delete[] drone.sensors[sensCount].name;
        }
    }

    for (int i = 0; i < drone.sensorCount; i++) {
        if (i != idx) {
            delete[] drone.sensors[i].name;
        }
    }
    delete[] drone.sensors;
    drone.sensors = newSensor;
    drone.sensorCount--;

    return true;
}
#pragma endregion
#pragma region Drones
Drone createDrone(const char* droneId, float maxSpeed) {
    Drone emptyDrone = {};
    emptyDrone.sensors = nullptr;
    if (!droneId || maxSpeed <= 0) return emptyDrone;
    Drone drone{};

    drone.droneId = new char[strlen(droneId) + 1];
    strcpy(drone.droneId, droneId);
    drone.maxSpeed = maxSpeed;
    drone.sensorCount = 0;
    drone.sensors = nullptr;

    return drone;
}

bool addDrone(BaseStation& station, const Drone& drone) {
    if (!station.name) return false;
    auto* newDrones = new Drone[station.droneCount + 1];

    for (int i = 0; i < station.droneCount; i++) {
        newDrones[i].maxSpeed = station.drones[i].maxSpeed;
        newDrones[i].droneId = new char[strlen(station.drones[i].droneId) + 1];
        strcpy(newDrones[i].droneId, station.drones[i].droneId);

        newDrones[i].sensorCount = station.drones[i].sensorCount;
        if (station.drones[i].sensorCount > 0) {
            newDrones[i].sensors = new Sensor[station.drones[i].sensorCount];

            for (int j = 0; j < station.drones[i].sensorCount; j++) {
                newDrones[i].sensors[j].name = new char[strlen(station.drones[i].sensors[j].name) + 1];
                strcpy(newDrones[i].sensors[j].name, station.drones[i].sensors[j].name);
                newDrones[i].sensors[j].accuracy = station.drones[i].sensors[j].accuracy;
                newDrones[i].sensors[j].energyUsage = station.drones[i].sensors[j].energyUsage;
            }
        }
        else {
            newDrones[i].sensors = nullptr;
        }
    }


    newDrones[station.droneCount].maxSpeed = drone.maxSpeed;
    newDrones[station.droneCount].droneId = new char[strlen(drone.droneId) + 1];
    strcpy(newDrones[station.droneCount].droneId, drone.droneId);

    newDrones[station.droneCount].sensorCount = drone.sensorCount;
    if (drone.sensorCount > 0) {
        newDrones[station.droneCount].sensors = new Sensor[drone.sensorCount];

        for (int i = 0; i < drone.sensorCount; i++) {
            newDrones[station.droneCount].sensors[i].name = new char[strlen(drone.sensors[i].name) + 1];
            strcpy(newDrones[station.droneCount].sensors[i].name, drone.sensors[i].name);
            newDrones[station.droneCount].sensors[i].accuracy = drone.sensors[i].accuracy;
            newDrones[station.droneCount].sensors[i].energyUsage = drone.sensors[i].energyUsage;
        }
    }
    else {
        newDrones[station.droneCount].sensors = nullptr;
    }


    for (int i = 0; i < station.droneCount; i++) {
        for (int j = 0; j < station.drones[i].sensorCount; j++) {
            delete[] station.drones[i].sensors[j].name;
        }
        delete[] station.drones[i].droneId;
        delete[] station.drones[i].sensors;
    }
    delete[] station.drones;
    station.drones = newDrones;
    station.droneCount++;

    return true;
}


bool removeDrone(BaseStation& station, const int idx) {
    if (!station.name) return false;
    if (idx < 0 || idx >= station.droneCount) return false;

    auto* newDrones = new Drone[station.droneCount - 1];
    int newIdx = 0;

    for (int index = 0; index < station.droneCount; index++) {
        if (index == idx) {
            for (int k = 0; k < station.drones[index].sensorCount; k++) {
                delete[] station.drones[index].sensors[k].name;
            }
            delete[] station.drones[index].sensors;
            delete[] station.drones[index].droneId;
            continue;
        }

        newDrones[newIdx].maxSpeed = station.drones[index].maxSpeed;
        newDrones[newIdx].droneId = new char[strlen(station.drones[index].droneId) + 1];
        strcpy(newDrones[newIdx].droneId, station.drones[index].droneId);

        newDrones[newIdx].sensorCount = station.drones[index].sensorCount;

        if (station.drones[index].sensorCount > 0) {
            newDrones[newIdx].sensors = new Sensor[station.drones[index].sensorCount];
            for (int j = 0; j < station.drones[index].sensorCount; j++) {
                newDrones[newIdx].sensors[j].name = new char[strlen(station.drones[index].sensors[j].name) + 1];
                strcpy(newDrones[newIdx].sensors[j].name, station.drones[index].sensors[j].name);
                newDrones[newIdx].sensors[j].accuracy = station.drones[index].sensors[j].accuracy;
                newDrones[newIdx].sensors[j].energyUsage = station.drones[index].sensors[j].energyUsage;
            }
        }
        else {
            newDrones[newIdx].sensors = nullptr;
        }
        newIdx++;
    }

    for (int j = 0; j < station.droneCount; j++) {
        if (j != idx) {
            for (int k = 0; k < station.drones[j].sensorCount; k++) {
                delete[] station.drones[j].sensors[k].name;
            }
            delete[] station.drones[j].sensors;
            delete[] station.drones[j].droneId;
        }
    }
    delete[] station.drones;

    station.drones = newDrones;
    station.droneCount--;
    return true;
}
#pragma endregion
#pragma region BaseStation
BaseStation createStation(const char* name, const float x, const float y) {
    BaseStation emptyStation{};
    if (!name) return emptyStation;

    BaseStation station{};
    station.name = new char[strlen(name) + 1];
    strcpy(station.name, name);
    station.x = x;
    station.y = y;
    station.droneCount = 0;
    station.drones = new Drone[station.droneCount];

    return station;
}


bool addStation(Network& network, const BaseStation& station) {
    if (!network.name || !station.drones) return false;
    auto* newStation = new BaseStation[network.stationCount + 1];

    for (int i = 0; i < network.stationCount; i++) {
        newStation[i].name = new char[strlen(network.stations[i].name) + 1];
        strcpy(newStation[i].name, network.stations[i].name);
        newStation[i].x = network.stations[i].x;
        newStation[i].y = network.stations[i].y;
        newStation[i].droneCount = network.stations[i].droneCount;
        if (network.stations[i].droneCount > 0) {
            newStation[i].drones = new Drone[network.stations[i].droneCount];

            for (int drCount = 0; drCount < network.stations[i].droneCount; drCount++) {
                newStation[i].drones[drCount].droneId = new char[strlen(network.stations[i].drones[drCount].droneId) + 1];
                strcpy(newStation[i].drones[drCount].droneId, network.stations[i].drones[drCount].droneId);
                newStation[i].drones[drCount].maxSpeed = network.stations[i].drones[drCount].maxSpeed;
                newStation[i].drones[drCount].sensorCount = network.stations[i].drones[drCount].sensorCount;

                if (network.stations[i].drones[drCount].sensorCount > 0) {
                    newStation[i].drones[drCount].sensors = new Sensor[network.stations[i].drones[drCount].sensorCount];

                    for (int sensCount = 0; sensCount < network.stations[i].drones[drCount].sensorCount; sensCount++) {
                        newStation[i].drones[drCount].sensors[sensCount].name = new char[strlen(network.stations[i].drones[drCount].sensors[sensCount].name) + 1];
                        strcpy(newStation[i].drones[drCount].sensors[sensCount].name, network.stations[i].drones[drCount].sensors[sensCount].name);
                        newStation[i].drones[drCount].sensors[sensCount].accuracy = network.stations[i].drones[drCount].sensors[sensCount].accuracy;
                        newStation[i].drones[drCount].sensors[sensCount].energyUsage = network.stations[i].drones[drCount].sensors[sensCount].energyUsage;
                    }
                }
                else {
                    newStation[i].drones[drCount].sensors = nullptr;
                }
            }
        }
        else {
            newStation[i].drones = nullptr;
        }
   }

    newStation[network.stationCount].x = station.x;
    newStation[network.stationCount].y = station.y;
    newStation[network.stationCount].name = new char[strlen(station.name) + 1];
    strcpy(newStation[network.stationCount].name, station.name);
    if (station.droneCount > 0) {
        newStation[network.stationCount].drones = new Drone[station.droneCount];

        for (int drCount = 0; drCount < station.droneCount; drCount++) {
            newStation[network.stationCount].drones[drCount].droneId = new char[strlen(station.drones[drCount].droneId) + 1];
            strcpy(newStation[network.stationCount].drones[drCount].droneId, station.drones[drCount].droneId);
            newStation[network.stationCount].drones[drCount].maxSpeed = station.drones[drCount].maxSpeed;

            if (station.drones[drCount].sensorCount > 0) {
                newStation[network.stationCount].drones[drCount].sensors = new Sensor[station.drones[drCount].sensorCount];

                for (int sensCount = 0; sensCount < station.drones[drCount].sensorCount; sensCount++) {
                    newStation[network.stationCount].drones[drCount].sensors[sensCount].name = new char[strlen(station.drones[drCount].sensors[sensCount].name) + 1];
                    strcpy(newStation[network.stationCount].drones[drCount].sensors[sensCount].name, station.drones[drCount].sensors[sensCount].name);
                    newStation[network.stationCount].drones[drCount].sensors[sensCount].accuracy = station.drones[drCount].sensors[sensCount].accuracy;
                    newStation[network.stationCount].drones[drCount].sensors[sensCount].energyUsage = station.drones[drCount].sensors[sensCount].energyUsage;
                }
                newStation[network.stationCount].drones[drCount].sensorCount = station.drones[drCount].sensorCount;
            }
            else {
                newStation[network.stationCount].drones[drCount].sensors = nullptr;
            }
        }
        newStation[network.stationCount].droneCount = station.droneCount;
    }
    else {
        newStation[network.stationCount].drones = nullptr;
    }

    for (int i = 0; i < network.stationCount; i++) {
        for (int j = 0; j < network.stations[i].droneCount; j++) {
            for (int k = 0; k < network.stations[i].drones[j].sensorCount; k++) {
                delete[] network.stations[i].drones[j].sensors[k].name;
            }
            delete[] network.stations[i].drones[j].sensors;
            delete[] network.stations[i].drones[j].droneId;
        }
        delete[] network.stations[i].drones;
        delete[] network.stations[i].name;
    }
    delete[] network.stations;
    network.stations = nullptr;

    network.stations = newStation;
    network.stationCount++;
    return true;
}


bool removeStation(Network& network, const int idx) {
    if (!network.name) return false;
    if (idx < 0 || idx >= network.stationCount) return false;

    auto* newStation = new BaseStation[network.stationCount - 1];

    int newIdx = 0;
    for (int stCount = 0; stCount < network.stationCount; stCount++) {
        if (stCount == idx) {
            for (int i = 0; i < network.stations[stCount].droneCount; i++) {
                for (int j = 0; j < network.stations[stCount].drones[i].sensorCount; j++) {
                    delete[] network.stations[stCount].drones[i].sensors[j].name;
                }
                delete[] network.stations[stCount].drones[i].droneId;
                delete[] network.stations[stCount].drones[i].sensors;
            }
            delete[] network.stations[stCount].drones;
            delete[] network.stations[stCount].name;
            continue;
        }


        newStation[newIdx].x = network.stations[stCount].x;
        newStation[newIdx].y = network.stations[stCount].y;
        newStation[newIdx].name = new char[strlen(network.stations[stCount].name) + 1];
        strcpy(newStation[newIdx].name, network.stations[stCount].name);
        newStation[newIdx].droneCount = network.stations[stCount].droneCount;

        if (newStation[newIdx].droneCount > 0) {
            newStation[newIdx].drones = new Drone[network.stations[stCount].droneCount];

            for (int drCount = 0; drCount < network.stations[stCount].droneCount; drCount++) {
                newStation[newIdx].drones[drCount].maxSpeed = network.stations[stCount].drones[drCount].maxSpeed;
                newStation[newIdx].drones[drCount].droneId = new char[strlen(network.stations[stCount].drones[drCount].droneId) + 1];
                strcpy(newStation[newIdx].drones[drCount].droneId, network.stations[stCount].drones[drCount].droneId);
                newStation[newIdx].drones[drCount].sensorCount = network.stations[stCount].drones[drCount].sensorCount;

                if (network.stations[stCount].drones[drCount].sensorCount > 0) {
                    newStation[newIdx].drones[drCount].sensors = new Sensor[network.stations[stCount].drones[drCount].sensorCount];

                    for (int sensCount = 0; sensCount < network.stations[stCount].drones[drCount].sensorCount; sensCount++) {
                        newStation[newIdx].drones[drCount].sensors[sensCount].name = new char[strlen(network.stations[stCount].drones[drCount].sensors[sensCount].name) + 1];
                        strcpy(newStation[newIdx].drones[drCount].sensors[sensCount].name, network.stations[stCount].drones[drCount].sensors[sensCount].name);
                        newStation[newIdx].drones[drCount].sensors[sensCount].accuracy = network.stations[stCount].drones[drCount].sensors[sensCount].accuracy;
                        newStation[newIdx].drones[drCount].sensors[sensCount].energyUsage = network.stations[stCount].drones[drCount].sensors[sensCount].energyUsage;
                    }
                }
                else {
                    newStation[newIdx].drones[drCount].sensors = nullptr;
                }
            }
        }
        else {
            newStation[newIdx].drones = nullptr;
        }
        newIdx++;
    }


    for (int i = 0; i < network.stationCount; i++) {
        if (i != idx) {
            for (int j = 0; j < network.stations[i].droneCount; j++) {
                for (int k = 0; k < network.stations[i].drones[j].sensorCount; k++) {
                    delete[] network.stations[i].drones[j].sensors[k].name;
                }
                delete[] network.stations[i].drones[j].sensors;
                delete[] network.stations[i].drones[j].droneId;
            }
            delete[] network.stations[i].drones;
            delete[] network.stations[i].name;
        }
    }
    delete[] network.stations;

    network.stations = newStation;
    network.stationCount--;
    return true;
}


#pragma endregion
#pragma region Network
Network createNetwork(const char* name) {
    Network network{};
    network.name = new char[strlen(name) + 1];
    strcpy(network.name, name);
    network.stationCount = 0;
    network.stations = new BaseStation[network.stationCount];
    return network;
}

bool addNetwork(const Network& network, System& system) {
    if (!network.name) return false;

    auto* newNetwork = new Network[system.networkCount + 1];

    for (int i = 0; i < system.networkCount; i++) {
        newNetwork[i].name = new char[strlen(system.networks[i].name) + 1];
        strcpy(newNetwork[i].name, system.networks[i].name);
        newNetwork[i].stationCount = 0;
        newNetwork[i].stations = nullptr;

        for (int j = 0; j < system.networks[i].stationCount; j++) {
            addStation(newNetwork[i], system.networks[i].stations[j]);
        }
    }

    newNetwork[system.networkCount].name = new char[strlen(network.name) + 1];
    strcpy(newNetwork[system.networkCount].name, network.name);
    newNetwork[system.networkCount].stationCount = 0;
    newNetwork[system.networkCount].stations = nullptr;

    for (int stCount = 0; stCount < network.stationCount; stCount++) {
        addStation(newNetwork[system.networkCount], network.stations[stCount]);
    }

    for (int i = 0; i < system.networkCount; i++) {
        for (int j = system.networks[i].stationCount - 1; j >= 0 ; j--) {
            removeStation(system.networks[i], j);
        }
        delete[] system.networks[i].name;
    }

    delete[] system.networks;
    system.networks = newNetwork;
    system.networkCount++;
    return true;
}

bool removeNetwork(System& system, const int& idx) {
    if (idx >= system.networkCount || idx < 0) return false;

    auto* newNetwork = new Network[system.networkCount - 1];

    int newIdx = 0;
    for (int i = 0; i < system.networkCount; i++) {
        if (i == idx) continue;
        newNetwork[newIdx].name = new char[strlen(system.networks[i].name) + 1];
        strcpy(newNetwork[newIdx].name, system.networks[i].name);
        newNetwork[newIdx].stationCount = 0;
        newNetwork[newIdx].stations = nullptr;

        for (int j = 0; j < system.networks[i].stationCount; j++) {
            addStation(newNetwork[newIdx], system.networks[i].stations[j]);
        }
        newIdx++;
    }


    for (int i = 0; i < system.networkCount; i++) {
        for (int j = system.networks[i].stationCount - 1; j >= 0; j--) {
            removeStation(system.networks[i], j);
        }
        delete[] system.networks[i].name;
    }

    delete[] system.networks;
    system.networks = newNetwork;
    system.networkCount--;
    return true;
}




#pragma endregion
#pragma region DopFucntions
float droneEnergyUsage(const Drone& drone) {
    float sum = 0;
    for (int i = 0; i < drone.sensorCount; i++) {
        sum += drone.sensors[i].energyUsage;
    }
    return sum;
}

Drone* findMaxSpeed(const BaseStation& station) {
    if (station.droneCount == 0) return nullptr;
    float maxSpeed = 0;
    int idx = -1;
    for (int i = 0; i < station.droneCount; i++) {
        if (station.drones[i].maxSpeed > maxSpeed) {
            maxSpeed = station.drones[i].maxSpeed;
            idx = i;
        }
    }
    if (idx == -1) return nullptr;
    return &station.drones[idx];
}
#pragma endregion
#pragma region Services
#pragma endregion


