///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the alternating disks
// problem.
//
// As provided, this header has four functions marked with TODO comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// State of one disk, either light or dark.
enum disk_color { DISK_LIGHT, DISK_DARK };

// Data structure for the state of one row of disks.
class disk_state {
private:
  std::vector<disk_color> _colors;

public:

  // takes in argument of light disks, creates array of double the size, and creates alternating array starting
  // with DISK_BLACK
  disk_state(size_t light_count)
    : _colors(light_count * 2, DISK_LIGHT) {

      assert(light_count > 0);

      for (size_t i = 0; i < _colors.size(); i += 2) {
        _colors[i] = DISK_DARK;
      }
  }

  // Equality operator for unit tests.
  bool operator== (const disk_state& rhs) const {
    return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
  }

  //returns total size
  size_t total_count() const {
    return _colors.size();
  }

  //assume light count is half of total
  size_t light_count() const {
    return total_count() / 2;
  }

  //assume dark count == light count
  size_t dark_count() const {
    return light_count();
  }

  //returns true or false if index is beyond size
  bool is_index(size_t i) const {
    return (i < total_count());
  }

  //returns color at index 'DISK_DARK' or 'DISK_LIGHT'
  disk_color get(size_t index) const {
    assert(is_index(index));
    return _colors[index];
  }

  //swaps the current index color with the color of the index to the right
  void swap(size_t left_index) {
	assert(is_index(left_index));
	auto right_index = left_index + 1;
	assert(is_index(right_index));
    std::swap(_colors[left_index], _colors[right_index]);
  }

  //returns string of 'L' or 'D' for light or dark
  std::string to_string() const {
    std::stringstream ss;
    bool first = true;
    for (auto color : _colors) {
      if (!first) {
        ss << " ";
      }

      if (color == DISK_LIGHT) {
        ss << "L";
      } else {
        ss << "D";
      }

      first = false;
    }
    return ss.str();
  }

  // Return true when this disk_state is in alternating format. That means
  // that the first disk at index 0 is dark, the second disk at index 1
  // is light, and so on for the entire row of disks.
  bool is_alternating() const {
	  //check first slot is dark, else it must be false
	  if (get(0) == DISK_DARK) {
		  // use int disk_color for DARK(0) and LIGHT(1)
		  // use int disk_index for current index
		  int disk_color = 0;
		  int disk_index = 0;
		  disk_index++;
		  while (is_index(disk_index)) {
			  //check if current index is dark and previous is dark, and fails it
			  if (get(disk_index) == DISK_DARK && disk_color == 0)
				  return false;
			  //checks if current index is light and if previous is light, and fails it
			  else if (get(disk_index) == DISK_LIGHT && disk_color == 1)
				  return false;
			  else {
				  disk_index++;
				  if (disk_color == 0)
					  disk_color++;
				  else
					  disk_color--;
			  }
		  }
	  }
	  else
		  return false;
    return true;
  }

  // Return true when this disk_state is fully sorted, with all light disks
  // on the left (low indices) and all dark disks on the right (high
  // indices).
  bool is_sorted() const {
	for (unsigned int i = 0; i < light_count(); i++)
	{
		if (get(i) == DISK_DARK)
			return false;
	}
	for (unsigned int i = light_count(); i < total_count(); i++)
	{
		if (get(i) == DISK_LIGHT)
			return false;
	}
	return true;
  }
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
  disk_state _after;
  unsigned _swap_count;

public:

  sorted_disks(const disk_state& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  sorted_disks(disk_state&& after, unsigned swap_count)
    : _after(after), _swap_count(swap_count) { }

  const disk_state& after() const {
    return _after;
  }

  unsigned swap_count() const {
    return _swap_count;
  }
};

// Algorithm that sorts disks using the left-to-right algorithm.
sorted_disks sort_left_to_right(const disk_state& before) {
	disk_state before_2 = before;
	int disk_swaps = 0;
	for (unsigned int a = 0; a < before_2.total_count(); a++) {
		for (unsigned int b = 0; b < before_2.total_count()-1; b++) {
			//swap dark disks rightwards and dont swap if both DISK_DARK
			if (before_2.get(b) == DISK_DARK && before_2.get(b+1) == DISK_LIGHT) {
				before_2.swap(b);
				disk_swaps++;
			}
		}
	}
	
  return sorted_disks(before_2, disk_swaps);
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {
	disk_state before_2 = before;
	int disk_swaps = 0;
	for (unsigned int a = 0; a < before_2.total_count(); a++) {
		for (unsigned int b = before_2.total_count()-1; b > a; b--) {
			//swap dark disks rightwards and dont swap if both DISK_DARK
			if (before_2.get(b) == DISK_LIGHT && before_2.get(b-1) == DISK_DARK) {
				before_2.swap(b-1);
				disk_swaps++;
			}
		}
	}

	return sorted_disks(before_2, disk_swaps);
}
