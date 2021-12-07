#include <MaybeBool.h>

bool MaybeBool::determined() const { return is_determined; }

bool MaybeBool::get_value() const
{
  if (is_determined)
  {
    return value;
  }
  else
  {
    throw std::domain_error("Indetermined value error");
  }
}

MaybeBool MaybeBool::operator!() const
{
  if (is_determined)
  {
    return MaybeBool(!value);
  }
  else
  {
    return MaybeBool(*this);
  }
}
