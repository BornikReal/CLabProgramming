#include <iostream>
#include <vector>
#include <string>
using namespace std;
#pragma once

class LinkedSet
{
private:
    struct cell
    {
        string value;
        cell *prev;
        cell *next;
    };
    vector<vector<cell *>> set_container;
    int size = 100;
    cell *last = nullptr;
    int hash(string x)
    {
        int hash_code = 0, num = 1;
        for (int i = 0; i < x.size(); i++)
        {
            hash_code = (hash_code + x[i] * num) % size;
            num = (num * 2) % size;
        }
        return hash_code;
    }
    int find(string x)
    {
        int search = hash(x);
        for (int i = 0; i < set_container[search].size(); i++)
        {
            if (set_container[search][i]->value == x)
                return i;
        }
        return 1;
    }
    string linked_set_members(cell *cur, string result = "")
    {
        if (cur != nullptr)
        {
            result += " " + cur->value;
            return linked_set_members(cur->prev, result);
        }
        return result;
    }
    void linked_set_deleteall(cell *cur)
    {
        if (cur != nullptr)
        {
            linked_set_deleteall(cur->prev);
        }
        delete cur;
    }

public:
    int num_of_mem = 0;
    LinkedSet()
    {
        set_container.resize(size);
    }
    int linked_set_put(string x)
    {
        int search = find(x);
        if (search == -1)
        {
            cell *new_cell = new cell{x, last, nullptr};
            if (last != nullptr)
                last->next = new_cell;
            last = new_cell;
            set_container[hash(x)].push_back(new_cell);
            num_of_mem++;
            return 0;
        }
        return 1;
    }
    int linked_set_delete(string x)
    {
        int search = find(x);
        if (search != -1)
        {
            cell *temp = set_container[hash(x)][search];
            if (temp->prev != nullptr)
                temp->prev->next = temp->next;
            if (temp->next != nullptr)
                temp->next->prev = temp->prev;
            if (last == temp)
                last = temp->prev;
            delete temp;
            set_container[hash(x)].erase(set_container[hash(x)].begin() + search);
            num_of_mem--;
            return 0;
        }
        return 1;
    }
    string linked_set_prev(string x)
    {
        int search = find(x);
        if (search == -1)
            return "none";
        cell *temp = set_container[hash(x)][search];
        if (temp->prev == nullptr)
            return "none";
        return temp->prev->value;
    }
    string linked_set_next(string x)
    {
        int search = find(x);
        if (search == -1)
            return "none";
        cell *temp = set_container[hash(x)][search];
        if (temp->next == nullptr)
            return "none";
        return temp->next->value;
    }
    string linked_set_members()
    {
        string result = to_string(num_of_mem);
        result += linked_set_members(last);
        return result;
    }
    int linked_set_deleteall()
    {
        linked_set_deleteall(last);
        int temp = num_of_mem;
        num_of_mem = 0;
        return temp;
    }
};

class MultiMap
{
private:
    struct cell
    {
        string key;
        LinkedSet values;
    };
    vector<vector<cell>> set_container;
    int size = 10000;
    int hash(string x)
    {
        int hash_code = 0, num = 1;
        for (int i = 0; i < x.size(); i++)
        {
            hash_code = (hash_code + x[i] * num) % size;
            num = (num * 2) % size;
        }
        return hash_code;
    }
    int find_key(string x)
    {
        int search = hash(x);
        for (int i = 0; i < set_container[search].size(); i++)
        {
            if (set_container[search][i].key == x)
                return i;
        }
        return -1;
    }

public:
    int num_of_members = 0;
    MultiMap()
    {
        set_container.resize(size);
    }
    void multi_map_put(string x, string y)
    {
        int search = find_key(x);
        if (search == -1)
        {
            cell new_cell;
            new_cell.key = x;
            new_cell.values.linked_set_put(y);
            set_container[hash(x)].push_back(new_cell);
            num_of_members++;
        }
        else
        {
            int ans = set_container[hash(x)][search].values.linked_set_put(y);
            if (!ans)
                num_of_members++;
        }
    }
    string multi_map_get(string x)
    {
        int search = find_key(x);
        if (search != -1)
            return set_container[hash(x)][search].values.linked_set_members();
        else
            return "0";
    }
    void multi_map_delete(string x, string y)
    {
        int search = find_key(x);
        if (search != -1)
        {
            int ans = set_container[hash(x)][search].values.linked_set_delete(y);
            if (!ans)
                num_of_members--;
            if (set_container[hash(x)][search].values.num_of_mem == 0)
            {
                set_container[hash(x)][search].values.linked_set_deleteall();
                set_container[hash(x)].erase(set_container[hash(x)].begin() + search);
            }
        }
    }
    void multi_map_deleteall(string x)
    {
        int search = find_key(x);
        if (search != -1)
        {
            num_of_members -= set_container[hash(x)][search].values.linked_set_deleteall();
            set_container[hash(x)].erase(set_container[hash(x)].begin() + search);
        }
    }
};