#include <random>
#include <iostream>

const double qualityModulePercent = 0.248;
const double uptierPercent = 0.1;
const double voidPercent = 0.25;
const bool rollEveryTime = false;

enum quality {
    VOID,
    NORMAL,
    UNCOMMON,
    RARE,
    EPIC,
    LEGENDARY
};

bool randomChance(double chance)
{
    static std::random_device rd;
    static std::mt19937 gen{ rd() };
    std::bernoulli_distribution distribution{ chance };
    return distribution(gen);
}

quality upgrade(quality before)
{
    if (before == NORMAL) return UNCOMMON;
    else if (before == UNCOMMON) return RARE;
    else if (before == RARE) return EPIC;
    else if (before == EPIC) return LEGENDARY;
    else return before; //legendary goes to legendary
}

quality craftGear(quality plateQuality)
{
    quality result = plateQuality;
    if (randomChance(qualityModulePercent))//we got a quality upgrade
    {
        result = upgrade(result);
        for (int i = 0; i < 4; i++)//uncommon->rare->epic-legendary
        {
            if (randomChance(uptierPercent))//we got a quality upgrade
            {
                result = upgrade(result);
            }
            else
            {
                break;
            }
        }
    }
    return result;
}
quality recycle(quality gearQuality)
{
    if (randomChance(voidPercent))//it didnt get voided
    {
        quality result = gearQuality;
        if (randomChance(qualityModulePercent))//we got a quality upgrade
        {
            result = upgrade(result);
            for (int i = 0; i < 4; i++)//uncommon->rare->epic-legendary
            {
                if (randomChance(uptierPercent))//we got a quality upgrade
                {
                    result = upgrade(result);
                }
                else
                {
                    break;
                }
            }
        }
        return result;
    }
    return VOID;
}

int main()
{
    long hits = 0;
    for (int tries = 0; tries < 1000000000; tries++)
    {
        quality gear = NORMAL;
        quality plate = NORMAL;
        while (gear != VOID && plate != VOID)//roll until legendary or voided
        {
            if (rollEveryTime)
            {
                gear = recycle(gear);
            }
            else
            {
                gear = craftGear(plate);
                plate = recycle(gear);
            }
            if (gear == LEGENDARY)
            {
                hits++;
                break;
            }
        }
        if (tries % 100000 == 0 && hits > 0)
        {
            std::cout << (double)tries / (double)hits <<" " << (double)hits / (double)tries << " tries: " << tries << " hits: " << hits << std::endl;
        }
    }
}