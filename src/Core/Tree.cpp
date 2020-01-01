#include "Core/Tree.h"

#include <iostream>

#include <iterator>
#include <unordered_set>

namespace Core
{
	Tree::Tree()
		: m_rootNodes(std::vector<TreeNode*>()), m_maxDepth(0)
	{
	}

	Tree::~Tree()
	{
	}

	double percentage = 0;
	unsigned int constResultNumber = 0;
	unsigned int currentResultIndex = 0;

	void Tree::CreateTree(const Profile::ProfileSerializeObject& profileObject)
	{
		PROFILE_FUNCTION();

		m_isReady = false;

		if (profileObject.traceEvents.empty() || profileObject.traceEvents.size() == 0)
		{
			return;
		}

		constResultNumber = (unsigned int)profileObject.traceEvents.size();

		TreeNode* baseNode = new TreeNode();
		baseNode->Parent = nullptr;
		baseNode->ProfileResult = profileObject.traceEvents[0];
		baseNode->x = 0;
		baseNode->y = 0;
		m_rootNodes.push_back(baseNode);

		currentResultIndex++;

		percentage = (double)currentResultIndex / (double)constResultNumber;
		std::cout << "Percentage: " << percentage << "\n";

		bool firstPass = true;
		int i = 0;
		for (size_t i = 1; i < profileObject.traceEvents.size(); i++)
		{
			firstPass = false;
			if (!AddNode(baseNode, profileObject.traceEvents[i]) && !firstPass)
			{
				// Add a new root node.
				TreeNode* newRootNode = new TreeNode();
				newRootNode->Parent = nullptr;
				newRootNode->ProfileResult = profileObject.traceEvents[i];
				newRootNode->x = (int)m_rootNodes.size();
				newRootNode->y = 0;
				m_rootNodes.push_back(newRootNode);

				//TreeNode newRootNode;
				//newRootNode.Parent = nullptr;
				//newRootNode.ProfileResult = profileObject.traceEvents[i];
				//newRootNode.x = m_rootNodes.size();
				//newRootNode.y = 0;
				//tempNodesArr[i] = newRootNode;

				currentResultIndex++;

				percentage = (double)currentResultIndex / (double)constResultNumber;
				std::cout << "Percentage: " << percentage << "\n";

				baseNode = newRootNode;
			}
		}

		m_isReady = true;
	}

	void Tree::Destroy()
	{
		for (auto it = m_rootNodes.begin(); it != m_rootNodes.end(); ++it)
		{
			DestoryNode(*it);
		}

		m_rootNodes.clear();
	}

	int indent = 0;

	std::string GetSpliter(const int& index)
	{
		std::string s;

		for (int i = 0; i < index; i++)
		{
			s.append("-");
		}

		return s;
	}

	void Tree::PrintTreeToConsole(TreeNode* node)
	{
		if (node == nullptr)
		{
			for (int i = 0; i < m_rootNodes.size(); ++i)
			{
				PrintTreeToConsole(m_rootNodes[i]);
				indent = 0;
			}
		}
		else
		{
			std::cout << GetSpliter(indent) << node->ProfileResult.Name << ", Y: " << node->y << ", X: " << node->x << ", dur: " << node->GetMilli() <<"\n";

			for (size_t i = 0; i < node->Children.size(); ++i)
			{
				indent += 2;
				PrintTreeToConsole(node->Children[i]);
			}

			indent -= 2;
		}
	}

	bool Tree::DestoryNode(TreeNode* node)
	{
		if (node == nullptr)
		{
			return false;
		}

		for (auto it = node->Children.begin(); it != node->Children.end(); ++it)
		{
			DestoryNode(*it);
		}

		node->Children.clear();
		delete node;

		return true;
	}

	bool Tree::AddNode(TreeNode* node, const Profile::ProfileResult& profileResult)
	{
		if (profileResult.Start >= node->ProfileResult.Start&& profileResult.End < node->ProfileResult.End)
		{
			for (auto it = node->Children.rbegin(); it != node->Children.rend(); ++it)
			{
				if (AddNode(*it, profileResult))
				{
					return true;
				}
				else
				{
					break;
				}
			}
			TreeNode* tNode = new TreeNode();
			tNode->Parent = node;
			tNode->ProfileResult = profileResult;
			tNode->y = node->y + 1;
			tNode->x = (int)node->Children.size();
			node->AddChild(tNode);

			//TreeNode tNode;
			//tNode.Parent = node;
			//tNode.ProfileResult = profileResult;
			//tNode.y = node->y + 1;
			//tNode.x = node->Children.size();
			//node->AddChild(&tNode);

			currentResultIndex++;

			percentage = (double)currentResultIndex / (double)constResultNumber;
			std::cout << "Percentage: " << percentage << "\n";

			return true;
		}

		return false;

	}

	bool Tree::IsSiblingOf(TreeNode* node1, TreeNode* node2)
	{
		if (node1->Parent != nullptr && node2->Parent != nullptr)
		{
			for (auto it : node1->Parent->Children)
			{
				if (it == node2)
				{
					return true;
				}
			}
		}
		//root nodes have been found
		return false;
	}

	unsigned int Tree::GetDepth(TreeNode* node)
	{
		return 0;
	}

	Profile::ProfileResult Tree::GetNextProfileResult()
	{
		return Profile::ProfileResult();
	}
}