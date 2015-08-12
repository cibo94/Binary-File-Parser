/**
 * @file file.cpp
 * @author Miroslav Cibulka
 * @brief Here is executable File class implemented
 */

#include <bfp.hpp>


namespace bfp
  {
      File::File(
          bfd *fd,
          const char *path,
          const char *target)
          :
          _fd{fd},
          _path{path},
          _target{target}
        {
          long storage_needed;
          long number_of_symbols;
          long i;

          Section _s_com(bfd_com_section_ptr);
          Section _s_und(bfd_und_section_ptr);
          Section _s_abs(bfd_abs_section_ptr);
          Section _s_ind(bfd_ind_section_ptr);
          push_back(_s_com);
          push_back(_s_und);
          push_back(_s_abs);
          push_back(_s_ind);
          for (asection *_sec = _fd->sections; _sec != NULL; _sec = _sec->next)
            {
              Section _s(_sec);
              push_back(_s);
            }

          storage_needed = bfd_get_symtab_upper_bound (_fd);

          if (storage_needed < 0)
            BFP_ASSERT();

          if (storage_needed == 0)
            return;

          symbol_table = (asymbol **) malloc(
              static_cast<size_t>(storage_needed));

          if ((number_of_symbols = bfd_canonicalize_symtab (_fd,
                                                            symbol_table)) < 0)
            BFP_ASSERT();

          for (i = 0; i < number_of_symbols; ++i)
            {

              auto _sec = end();
              if (symbol_table[i]->section != nullptr)
                _sec = find(begin(), end(), symbol_table[i]->section);
              if (_sec == end())
                {
                  RAISE(Exception::Parser::WrongFormat);
                }
              else
                {
                  Symbol _sym(symbol_table[i]);
                  _sym._section = _sec;
                  _sec->push_back(_sym);
                }
            }
        }

      File::~File()
        {
          bfd_close(_fd);
          free(symbol_table);
        }

      ::std::vector<Section>::const_iterator File::cbegin()
        {
          return _sections.cbegin();
        }

      ::std::vector<Section>::const_iterator File::cend()
        {
          return _sections.cend();
        }

      ::std::vector<Section>::iterator File::begin()
        {
          return _sections.begin();
        }

      ::std::vector<Section>::iterator File::end()
        {
          return _sections.end();
        }

      ::std::vector<Section>::const_reverse_iterator File::crbegin()
        {
          return _sections.crbegin();
        }

      ::std::vector<Section>::const_reverse_iterator File::crend()
        {
          return _sections.crend();
        }

      ::std::vector<Section>::reverse_iterator File::rbegin()
        {
          return _sections.rbegin();
        }

      ::std::vector<Section>::reverse_iterator File::rend()
        {
          return _sections.rend();
        }

      size_t File::capacity()
        {
          return _sections.capacity();
        }

      size_t File::size()
        {
          return _sections.size();
        }

      size_t File::max_size()
        {
          return _sections.max_size();
        }

      Section File::operator[](size_t n)
        {
          return _sections[n];
        }

      Section File::front()
        {
          return _sections.front();
        }

      Section File::back()
        {
          return _sections.back();
        }

      Section File::at(size_t n)
        {
          return _sections.at(n);
        }

      bool File::empty()
        {
          return _sections.empty();
        }

      void File::push_back(Section &_sec)
        {
          _sections.push_back(_sec);
        }

      const char *File::get_path() const
        {
          return _path;
        }

      const char *File::get_target() const
        {
          return _target;
        }
  }