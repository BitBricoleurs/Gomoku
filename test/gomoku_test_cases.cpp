#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

std::string readTestFile(const std::string& fileName) {
    std::ifstream file(fileName);
    std::string line, content;
    while (getline(file, line)) {
        content += line + "\n";
    }
    return content;
}

#include <unistd.h>
#include <fcntl.h>
#include <chrono>

std::string executeAI(const std::string& commands, int timeoutSeconds = 6)
{
    int pipe_in[2], pipe_out[2];
    pipe(pipe_in);
    pipe(pipe_out);

    fcntl(pipe_out[0], F_SETFL, O_NONBLOCK);

    pid_t pid = fork();
    if (pid == 0) {
        dup2(pipe_in[0], STDIN_FILENO);
        dup2(pipe_out[1], STDOUT_FILENO);
        close(pipe_in[1]);
        close(pipe_out[0]);

        execlp("./pbrain-gomoku-ai", "./pbrain-gomoku-ai", (char *) NULL);
        exit(0);
    } else {
        close(pipe_in[0]);
        close(pipe_out[1]);

        write(pipe_in[1], commands.c_str(), commands.length());
        close(pipe_in[1]);

        auto startTime = std::chrono::high_resolution_clock::now();
        std::string aiResponse, line;
        char buffer[256];
        int numBytes;
        int newlineCount = 0;

        while (true) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);
            if (elapsed.count() >= timeoutSeconds) {
                close(pipe_out[0]);
                waitpid(pid, nullptr, 0);
                return "";
            }

            numBytes = read(pipe_out[0], buffer, sizeof(buffer) - 1);
            if (numBytes > 0) {
                buffer[numBytes] = '\0';
                aiResponse += buffer;

                newlineCount += std::count(aiResponse.begin(), aiResponse.end(), '\n');
                if (newlineCount >= 2) {
                    break;
                }
            }
            usleep(100000);
        }

        close(pipe_out[0]);
        waitpid(pid, nullptr, 0);
        return aiResponse;
    }
}



TEST(GomokuTest, test_winning_move) {
    std::string commands = readTestFile("scripts/test_winning_move.txt");
    if (commands.empty()) {
        FAIL() << "Test file not found";
    }

    std::string aiResponse = executeAI(commands);
    EXPECT_EQ(aiResponse, "OK\n0,4\n");
}

TEST(GomokuTest, test_defense_decision) {
    std::string commands = readTestFile("scripts/test_defense_decision.txt");
    if (commands.empty()) {
        FAIL() << "Test file not found";
    }

    std::string aiResponse = executeAI(commands);
    EXPECT_EQ(aiResponse, "OK\n4,1\n");
}

TEST(GomokuTest, StrategicResponseTest) {
    std::string commands = readTestFile("scripts/opportunity_creation_test.txt");
    ASSERT_FALSE(commands.empty()) << "Test file not found";

    std::string aiResponse = executeAI(commands);
    EXPECT_EQ(aiResponse, "OK\n5,4\n");
}

TEST(GomokuTest, ComplexConfig) {
    std::string commands = readTestFile("scripts/complex_configuration_test.txt");
    ASSERT_FALSE(commands.empty()) << "Test file not found";

    std::string aiResponse = executeAI(commands);
    EXPECT_EQ(aiResponse, "OK\n8,10\n");
}

TEST(GomokuTest, AnticipationTrap) {
    std::string commands = readTestFile("scripts/anticipation_trap_test.txt");
    ASSERT_FALSE(commands.empty()) << "Test file not found";

    std::string aiResponse = executeAI(commands);
    EXPECT_EQ(aiResponse, "OK\n8,8\n");
}

TEST(GomokuTest, CriticalSituation) {
    std::string commands = readTestFile("scripts/critical_situation_response_test.txt");
    ASSERT_FALSE(commands.empty()) << "Test file not found";

    std::string aiResponse = executeAI(commands);
    EXPECT_EQ(aiResponse, "OK\n14,14\n");
}

TEST(GomokuTest, MultipleOpportunities1) {
    std::string commands = readTestFile("scripts/multiple_opportunities_management_test.txt");
    ASSERT_FALSE(commands.empty()) << "Test file not found";

    std::string aiResponse = executeAI(commands);
    EXPECT_EQ(aiResponse, "OK\n3,5\n");
}

TEST(GomokuTest, MultipleOpportunities2) {
    std::string commands = readTestFile("scripts/multiple_opportunities_management_test2.txt");
    ASSERT_FALSE(commands.empty()) << "Test file not found";

    std::string aiResponse = executeAI(commands);
    EXPECT_EQ(aiResponse, "OK\n4,5\n");
}

TEST(GomokuTest, JesusTest) {
    std::string commands = readTestFile("scripts/test_jesus_test.txt");
    ASSERT_FALSE(commands.empty()) << "Test file not found";

    std::string aiResponse = executeAI(commands);
    EXPECT_EQ(aiResponse, "OK\n3,2\n");
}

TEST(GomokuTest, DefenseAndAttack) {
    std::string commands = readTestFile("scripts/defense_and_counterattack_test.txt");
    ASSERT_FALSE(commands.empty()) << "Test file not found";

    std::string aiResponse = executeAI(commands);
    EXPECT_EQ(aiResponse, "OK\n5,3\n");
}

TEST(GomokuTest, Defense) {
std::string commands = readTestFile("scripts/test_defense.txt");
ASSERT_FALSE(commands.empty()) << "Test file not found";

std::string aiResponse = executeAI(commands);
EXPECT_EQ(aiResponse, "OK\n5,5\n");
}
