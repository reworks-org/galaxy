///
/// VirtualFileSystemTest.cpp
/// Tests
///
/// Refer to LICENSE.txt for more details.
///

#include <gtest/gtest.h>

#include <galaxy/fs/VirtualFileSystem.hpp>

using namespace galaxy;

#define ROOT "temp/"

#define SET_UP   std::filesystem::create_directories(ROOT)
#define CLEAN_UP std::filesystem::remove_all(ROOT)

TEST(VirtualFileSystem, RootFile)
{
	EXPECT_THROW(
		{
			try
			{
				fs::VirtualFileSystem vfs {"test.txt"};
			}
			catch (const std::runtime_error& e)
			{
				ASSERT_FALSE(std::strcmp(e.what(), ""));
				throw;
			}
		},
		std::runtime_error);
}

TEST(VirtualFileSystem, RootDir)
{
	EXPECT_NO_THROW({
		try
		{
			fs::VirtualFileSystem vfs {ROOT};
		}
		catch (const std::runtime_error& e)
		{
			ASSERT_FALSE(std::strcmp(e.what(), ""));
			throw;
		}
	});
}

TEST(VirtualFileSystem, FindFolder)
{
	fs::VirtualFileSystem vfs {ROOT};

	const auto res = vfs.find("folder/");

	EXPECT_EQ(res, std::nullopt);
}

TEST(VirtualFileSystem, Find)
{
}

TEST(VirtualFileSystem, CreateFile)
{
	SET_UP;

	fs::VirtualFileSystem vfs {ROOT};

	auto res = vfs.make("empty.txt");
	EXPECT_TRUE(res);

	CLEAN_UP;
}

TEST(VirtualFileSystem, CreateFileFolder)
{
	SET_UP;

	fs::VirtualFileSystem vfs {ROOT};

	bool res = vfs.make("dir/empty.txt");
	ASSERT_TRUE(res);

	bool was_folder_created = std::filesystem::exists(ROOT + std::string {"dir"});
	EXPECT_TRUE(was_folder_created);

	CLEAN_UP;
}

TEST(VirtualFileSystem, CreateFileThatExists)
{
	SET_UP;

	fs::VirtualFileSystem vfs {ROOT};

	bool res = vfs.make("empty.txt");
	ASSERT_TRUE(res);

	bool res2 = vfs.make("empty.txt");
	EXPECT_FALSE(res2);

	CLEAN_UP;
}

TEST(VirtualFileSystem, CreateFolder)
{
	SET_UP;

	fs::VirtualFileSystem vfs {ROOT};

	auto res = vfs.make("empty");
	EXPECT_TRUE(res);

	CLEAN_UP;
}

TEST(VirtualFileSystem, CreateFolders)
{
	SET_UP;

	fs::VirtualFileSystem vfs {ROOT};

	bool res = vfs.make("dir/empty");
	ASSERT_TRUE(res);

	bool was_folder_created = std::filesystem::exists(ROOT + std::string {"dir"});
	EXPECT_TRUE(was_folder_created);

	CLEAN_UP;
}

TEST(VirtualFileSystem, CreateFolderThatExists)
{
	SET_UP;

	fs::VirtualFileSystem vfs {ROOT};

	bool res = vfs.make("empty");
	ASSERT_TRUE(res);

	bool res2 = vfs.make("empty");
	EXPECT_FALSE(res2);

	CLEAN_UP;
}

TEST(VirtualFileSystem, OpenExisting)
{
	SET_UP;

	fs::VirtualFileSystem vfs {ROOT};
	vfs.open("test.txt");

	CLEAN_UP;
}

TEST(VirtualFileSystem, OpenBinaryExisting)
{
	SET_UP;

	fs::VirtualFileSystem vfs {ROOT};

	CLEAN_UP;
}