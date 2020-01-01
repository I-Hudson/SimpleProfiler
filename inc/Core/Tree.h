#pragma once

#include "Profile/Instrumentor.h"
#include <vector>

namespace Core
{
	struct TreeNode
	{
		TreeNode* Parent;
		std::vector<TreeNode*> Children;
		Profile::ProfileResult ProfileResult;

		int x, y;

		void AddChild(TreeNode* node)
		{
			Children.push_back(node);
		}

		uint32_t GetMircoStart()
		{
			return (uint32_t)ProfileResult.Start;
		}

		uint32_t GetMircoEnd()
		{
			return (uint32_t)ProfileResult.End;
		}

		uint32_t GetMilliStart()
		{
			return (uint32_t)(ProfileResult.Start / 1000);
		}

		uint32_t GetMilliEnd()
		{
			return (uint32_t)(ProfileResult.End / 1000);
		}

		uint32_t GetMilli()
		{
			return GetMilliEnd() - GetMilliStart();
		}

	};

	class Tree
	{
	public:
		Tree();
		~Tree();

		void CreateTree(const Profile::ProfileSerializeObject& profileObject);
		void Destroy();

		bool IsReady() const { return m_isReady; }

		void PrintTreeToConsole(TreeNode* node);

		TreeNode* GetFirstRootNode()const { return m_isReady ? m_rootNodes[0] : nullptr; }
		TreeNode* GetLastRootNode()const { return m_isReady ? m_rootNodes[m_rootNodes.size() - 1] : nullptr; }

		std::vector<TreeNode*> m_rootNodes;
	private:

		bool DestoryNode(TreeNode* node);
		bool AddNode(TreeNode* node, const Profile::ProfileResult& profileResult);

		bool IsSiblingOf(TreeNode* node1, TreeNode* node2);

		unsigned int GetDepth(TreeNode* node);
		unsigned int GetMaxDepth() const { return m_maxDepth; }

		bool m_isReady;
		Profile::ProfileResult GetNextProfileResult();

	private:
		unsigned int m_maxDepth;
	};
}