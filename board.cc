#include "board.h"

#include "slc.h"
#include "tuition.h"
#include "tims.h"
#include "needlesHall.h"
#include "otherNonPayableBuildings.h"
#include "observer.h"
#include "gym.h"
#include "residence.h"

using namespace std;

void Board::addAcademicBuilding(int i) {
    // push the ith academicBuilding to the board
    shared_ptr<AcademicBuilding> cell = make_shared<AcademicBuilding>
            (blocks[i], names[i], nullptr, academicBuildingInfo[i] + 2, academicBuildingInfo[i][0], academicBuildingInfo[i][1]);
    for (auto it : cells) {
        if (!it) continue;
        if (it->getType() == Cell::Academic) {
            auto temp = dynamic_pointer_cast<AcademicBuilding>(it);
            if (temp && temp->getBlock() == cell->getBlock()) {
                cell->attach(temp.get());
                temp->attach(cell.get());
            }
        }
    }
    cells.push_back(cell);
}

Board::Board() : blocks{AcademicBuilding::Arts1, AcademicBuilding::Arts1, AcademicBuilding::Arts2, AcademicBuilding::Arts2,
                        AcademicBuilding::Arts2, AcademicBuilding::Eng, AcademicBuilding::Eng, AcademicBuilding::Eng,
                        AcademicBuilding::Health, AcademicBuilding::Health, AcademicBuilding::Health, AcademicBuilding::Env,
                        AcademicBuilding::Env, AcademicBuilding::Env, AcademicBuilding::Sci1, AcademicBuilding::Sci1,
                        AcademicBuilding::Sci1, AcademicBuilding::Sci2, AcademicBuilding::Sci2, AcademicBuilding::Sci2,
                        AcademicBuilding::Math, AcademicBuilding::Math},
                  names{"AL", "ML", "ECH", "PAS", "HH", "RCH", "DWE", "CPH", "LHI",
                        "BMH", "OPT", "EV1", "EV2", "EV3", "PHYS", "B1", "B2",
                        "EIT", "ESC", "C2", "MC", "DC"},
                 academicBuildingInfo{
                         {40, 50, 2, 10, 30, 90, 160, 250},
                         {60, 50, 4, 20, 60, 180, 320, 450},
                         {100, 50, 6, 30, 90, 270, 400, 550},
                         {100, 50, 6, 30, 90, 270, 400, 550},
                         {120, 50, 8, 40, 100, 300, 450, 600},
                         {140,100,10,50,150,450,625,750},
                         {140,100,10,50,150,450,625,750},
                         {160,100,12,60,180,500,700,900},
                         {180,100,14,70,200,550,750,950},
                         {180,100,14,70,200,550,750,950},
                         {200,100,16,80,220,600,800,1000},
                         {220,150,18,90,250,700,875,1050},
                         {220,150,18,90,250,700,875,1050},
                         {240,150,20,100,300,750,925,1100},
                         {260,150,22,110,330,800,975,1150},
                         {260,150,22,110,330,800,975,1150},
                         {280,150,24,120,360,850,1025,1200},
                         {300,200,26,130,390,900,1100,1275},
                         {300,200,26,130,390,900,1100,1275},
                         {320,200,28,150,450,1000,1200,1400},
                         {350,200,35,175,500,1100,1300,1500},
                         {400,200,50,200,600,1400,1700,2000}
                 } {
    int academicBuildingIndex = 0;

    // init residences
    vector<int> residenceCharge = {25, 50, 100, 200};
    vector<shared_ptr<Residence>> residences;
    auto mkv = make_shared<Residence>("MKV", nullptr, 200, residenceCharge);
    auto uwp = make_shared<Residence>("UWP", nullptr, 200, residenceCharge);
    auto v1 = make_shared<Residence>("V1", nullptr, 200, residenceCharge);
    auto rev = make_shared<Residence>("REV", nullptr, 200, residenceCharge);
    residences.push_back(mkv);
    residences.push_back(uwp);
    residences.push_back(v1);
    residences.push_back(rev);

    for (int i = 0; i < residences.size(); i++) {
        for (int j = 0; j < residences.size(); j++) {
            if (j == i) continue;
            residences.at(i)->attach(residences.at(j).get());
        }
    }

    // Bottom
    cells.push_back(make_shared<OtherNonPayableBuildings>("COLLECT OSAP"));
    addAcademicBuilding(academicBuildingIndex++);
    cells.push_back(make_shared<SLC>());
    addAcademicBuilding(academicBuildingIndex++);
    cells.push_back(make_shared<Tuition>());

    // Residence 1
    cells.push_back(residences.at(0));

    addAcademicBuilding(academicBuildingIndex++);
    cells.push_back(make_shared<NeedlesHall>());
    addAcademicBuilding(academicBuildingIndex++);
    addAcademicBuilding(academicBuildingIndex++);
    cells.push_back(make_shared<OtherNonPayableBuildings>("DC Tims Line"));
    // Left
    addAcademicBuilding(academicBuildingIndex++);

    // Gym 1
    auto pac = make_shared<Gym>("PAC", nullptr, 150);
    cells.push_back(pac);

    addAcademicBuilding(academicBuildingIndex++);
    addAcademicBuilding(academicBuildingIndex++);

    // Residence 2
    cells.push_back(residences.at(1));

    addAcademicBuilding(academicBuildingIndex++);

    cells.push_back(make_shared<SLC>());

    addAcademicBuilding(academicBuildingIndex++);
    addAcademicBuilding(academicBuildingIndex++);
    cells.push_back(make_shared<OtherNonPayableBuildings>("Goose Nesting"));
    // Top
    addAcademicBuilding(academicBuildingIndex++);
    cells.push_back(make_shared<NeedlesHall>());
    addAcademicBuilding(academicBuildingIndex++);
    addAcademicBuilding(academicBuildingIndex++);

    // residence 3
    cells.push_back(residences.at(2));


    addAcademicBuilding(academicBuildingIndex++);
    addAcademicBuilding(academicBuildingIndex++);

    auto cif = make_shared<Gym>("CIF", nullptr, 150);
    cif->attach(pac.get());
    pac->attach(pac.get());
    cells.push_back(cif);

    addAcademicBuilding(academicBuildingIndex++);
    cells.push_back(make_shared<OtherNonPayableBuildings>("GO TO TIMS"));

    // Right
    addAcademicBuilding(academicBuildingIndex++);
    addAcademicBuilding(academicBuildingIndex++);
    cells.push_back(make_shared<SLC>());
    addAcademicBuilding(academicBuildingIndex++);

    // residence 4
    cells.push_back(residences.at(3));

    cells.push_back(make_shared<NeedlesHall>());
    addAcademicBuilding(academicBuildingIndex++);
    cells.push_back(make_shared<OtherNonPayableBuildings>("COOP FEE"));
    addAcademicBuilding(academicBuildingIndex++);
}

Board::~Board() {}

shared_ptr<Cell> Board::getCellAt(int index) const {
    return cells.at(index);
}

std::shared_ptr<Cell> Board::getCell(std::string name) const {
    for (auto c : cells) {
        if (!c) continue;
        if (c->getName() == name) {
            return c;
        }
    }
    return nullptr;
}
