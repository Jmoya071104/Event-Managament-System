#pragma once
#include "EventManager.h"
#include "EventClass.h"

using namespace System;

// Provides 8 realistic sample events across all categories.
// Called when no events.txt data file exists (first run or reset).
public ref class SeedData
{
public:
    static void Seed(EventManager^ manager)
    {
        // Music
        manager->addEvent(gcnew EventClass(
            L"Summer Jazz Night",
            L"City Arts Foundation",
            L"Riverside Amphitheater, Building A",
            L"An outdoor jazz evening featuring local and regional artists. "
            L"Bring a blanket and enjoy the warm breeze as live jazz fills the air. "
            L"Food trucks will be on-site.",
            EventCategories::Music,
            DateTime(2026, 6, 14, 19, 0, 0),
            DateTime(2026, 6, 14, 22, 30, 0),
            300,
            L"",
            nullptr));

        // Sports
        manager->addEvent(gcnew EventClass(
            L"Campus 5K Fun Run",
            L"Campus Recreation Department",
            L"Main Quad, North Entrance",
            L"Open to all skill levels. The 5K route goes through campus and "
            L"the adjacent park trail. Awards for top finishers in each age group. "
            L"Registration includes a t-shirt.",
            EventCategories::Sports,
            DateTime(2026, 5, 2, 8, 0, 0),
            DateTime(2026, 5, 2, 11, 0, 0),
            200,
            L"",
            nullptr));

        // Tech
        manager->addEvent(gcnew EventClass(
            L"HCI & AI Design Workshop",
            L"Prof. Maria Chen",
            L"Engineering Hall, Room 205",
            L"A hands-on workshop exploring the intersection of human-computer "
            L"interaction and modern AI tools. Participants will prototype an "
            L"AI-assisted interface and present to peers. Laptops required.",
            EventCategories::Tech,
            DateTime(2026, 5, 10, 14, 0, 0),
            DateTime(2026, 5, 10, 17, 0, 0),
            30,
            L"",
            nullptr));

        // Art
        manager->addEvent(gcnew EventClass(
            L"Student Art Exhibition: New Voices",
            L"Department of Fine Arts",
            L"Gallery 7, Student Union",
            L"A curated showcase of undergraduate and graduate student work "
            L"spanning painting, sculpture, digital art, and photography. "
            L"Opening reception with light refreshments.",
            EventCategories::Art,
            DateTime(2026, 4, 25, 18, 0, 0),
            DateTime(2026, 4, 25, 21, 0, 0),
            0,   // unlimited
            L"",
            nullptr));

        // Food
        manager->addEvent(gcnew EventClass(
            L"International Street Food Festival",
            L"Global Student Association",
            L"University Commons, South Lawn",
            L"Sample dishes from over 20 countries prepared by international "
            L"student organizations. Cooking demonstrations, cultural performances, "
            L"and a best-dish competition judged by the audience.",
            EventCategories::Food,
            DateTime(2026, 5, 17, 11, 0, 0),
            DateTime(2026, 5, 17, 16, 0, 0),
            500,
            L"",
            nullptr));

        // Academic
        manager->addEvent(gcnew EventClass(
            L"Research Symposium 2026",
            L"Office of Undergraduate Research",
            L"Science Center Atrium",
            L"Annual showcase of student-led research projects across all "
            L"disciplines. Poster sessions, oral presentations, and keynote "
            L"from an invited industry researcher. Open to all students and faculty.",
            EventCategories::Academic,
            DateTime(2026, 4, 30, 9, 0, 0),
            DateTime(2026, 4, 30, 17, 0, 0),
            0,   // unlimited
            L"",
            nullptr));

        // Tech (second)
        manager->addEvent(gcnew EventClass(
            L"Game Dev Hackathon",
            L"Computer Science Society",
            L"Tech Hub, Floor 3",
            L"48-hour game development hackathon. Form teams of up to 4 and "
            L"build a playable game demo from scratch. Prizes for Best Gameplay, "
            L"Best Art, and Best Story. Snacks and energy drinks provided.",
            EventCategories::Tech,
            DateTime(2026, 5, 22, 17, 0, 0),
            DateTime(2026, 5, 24, 17, 0, 0),
            80,
            L"",
            nullptr));

        // Music (second)
        manager->addEvent(gcnew EventClass(
            L"Spring A Cappella Showcase",
            L"University Vocal Ensemble",
            L"Performing Arts Center, Main Stage",
            L"End-of-semester showcase featuring all five campus a cappella "
            L"groups. From pop to classical, each group performs a 15-minute "
            L"set. Doors open 30 minutes before showtime.",
            EventCategories::Music,
            DateTime(2026, 5, 8, 19, 30, 0),
            DateTime(2026, 5, 8, 21, 30, 0),
            250,
            L"",
            nullptr));
    }
};
