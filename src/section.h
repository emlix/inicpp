#ifndef INICPP_SECTION_H
#define INICPP_SECTION_H

#include <vector>
#include <iostream>
#include <iterator>

#include "exception.h"
#include "option.h"
#include "section_schema.h"

namespace inicpp
{
	/** Forward declaration, stated because of ring dependencies */
	class section_schema;
	/** Forward declaration of iterator used in section class */
	template<typename Element> class section_iterator;
	

	/**
	 * Represents section from ini format. Can contain multiple options.
	 * Always should be in config container class.
	 */
	class section
	{
	private:
		/** List of options in this instance */
		std::vector<option> options_;
		/** Name of this section */
		std::string name_;
		/** Corresponding section_schema if any */
		std::shared_ptr<section_schema> section_schema_;

		friend class section_iterator<option>;
		friend class section_iterator<const option>;

	public:
		typedef section_iterator<option> iterator;
		typedef section_iterator<const option> const_iterator;

		/**
		 * Default constructor is deleted.
		 */
		section() = delete;
		/**
		 * Copy constructor.
		 */
		section(const section &source);
		/**
		 * Copy assignment.
		 */
		section& operator=(const section &source);
		/**
		 * Move constructor.
		 */
		section(section &&source);
		/**
		 * Move assignment.
		 */
		section& operator=(section &&source);

		/**
		 * Construct instance of section class with given name.
		 * @param name name of newly created section class
		 */
		section(const std::string &name);
		
		/**
		 * Getter for name of this section.
		 * @return name
		 */
		const std::string &get_name() const;

		/**
		 * Creates and add option to this section.
		 * @param name name of newly created option class
		 * @param value value which will be stored in option
		 * @throws ambiguity_exception if option with specified name exists
		 */
		template<typename ValueType> void add_option(
			const std::string &option_name, const ValueType &value)
		{
			throw not_implemented_exception();
		}
		/**
		 * Add given option instance to options container.
		 * @param opt particular instance of option class
		 * @throws ambiguity_exception if option with specified name exists
		 */
		void add_option(const option &opt);
		/**
		 * From list of options remove the one with specified name
		 * @param option_name name of option which will be removed
		 * @throws not_found_exception if option with given name was not found
		 */
		void remove_option(const std::string &option_name);

		/**
		 * Returns size of options list
		 * @return unsigned integer
		 */
		size_t size();
		/**
		 * Access option on specified index.
		 * @param index
		 * @return modifiable reference to stored option
		 * @throws not_found_exception in case of out of range
		 */
		option &operator[](size_t index);
		/**
		 * Access option with specified name
		 * @param option_name
		 * @return modifiable reference to stored option
		 * @throws not_found_exception if option with given name does not exist
		 */
		option &operator[](const std::string &option_name);

		/**
		 * Validates this section agains given section_schema.
		 * @param sect_schema rules how this section should look like
		 * @param mode validation mode
		 * @return true if this section comply given section_schema
		 */
		bool validate(const section_schema &sect_schema, schema_mode mode);

		/**
		 * Classic stream operator for printing this instance to output stream.
		 * @param os output stream
		 * @return reference to output stream which allows chaining
		 */
		std::ostream &operator<<(std::ostream &os);

		/**
		 * Iterator pointing at the beginning of options list.
		 * @return section_iterator
		 */
		iterator begin();
		/**
		 * Iterator pointing at the end of options list.
		 * @return section_iterator
		 */
		iterator end();
		/**
		 * Constant iterator pointing at the beginning of options list.
		 * @return section_iterator
		 */
		const_iterator cbegin() const;
		/**
		 * Constant iterator pointing at the end of options list.
		 * @return section_iterator
		 */
		const_iterator cend() const;
	};
	

	/**
	 * Templated section iterator.
	 * Templates provide const and non-const iterator in one implementation.
	 * For easier implementation inheritance from std::iterator is used.
	 */
	template<typename Element>
	class section_iterator
		: public std::iterator<std::random_access_iterator_tag, Element>
	{
	private:
		using typename std::iterator<std::random_access_iterator_tag, Element>::reference;
		
		/** Reference to container which can be iterated */
		section &container_;
		/** Position in iterable container */
		size_t position_;
	public:
		/**
		 * Deleted default constructor.
		 */
		section_iterator() = delete;
		/**
		 * Copy constructor.
		 */
		section_iterator(const section_iterator &source) = default;
		/**
		 * Copy assignment.
		 */
		section_iterator& operator=(const section_iterator &source) = default;
		/**
		 * Move constructor.
		 */
		section_iterator(section_iterator &&source) = default;
		/**
		 * Move assignment.
		 */
		section_iterator& operator=(section_iterator &&source) = default;

		/**
		 * Construct iterator on given container
		 * and pointing at specified postion.
		 * @param source container which can be iterated
		 * @param position initial position to given container
		 */
		section_iterator(section &source, size_t position)
			: container_(source), position_(position) {}
		/**
		 * Construct iterator on given container pointing at the start.
		 * @param source container which can be iterated
		 */
		section_iterator(section &source) : section_iterator(source, 0) {}

		/**
		 * Moves iterator to next position.
		 * @return iterator itself
		 */
		section_iterator &operator++()
		{
			++position_;
			return *this;
		}
		/**
		 * Moves iterator to next position.
		 * @return new iterator with old position
		 */
		section_iterator operator++(int)
		{
			section_iterator old(*this);
			operator++();
			return old;
		}

		/**
		 * Equality compare method for iterators.
		 * @param second
		 * @return true if iterators are the same
		 */
		bool operator==(const section_iterator &second)
		{
			return this == &second && position_ == second.position_;
		}
		/**
		 * Non-equality compare method for iterators.
		 * @param second
		 * @return true if iterators are different
		 */
		bool operator!=(const section_iterator &second)
		{
			return !(*this == second);
		}

		/**
		 * Iterator dereference operator.
		 * @return reference to option on current position
		 */
		reference operator*()
		{
			return container_.options_.at(position_);
		}
	};
}

#endif
