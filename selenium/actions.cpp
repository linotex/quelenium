#include "actions.h"
#include "webdriverhub.h"


Mouse* Actions::mouse()
{
    return new Mouse(m_hub);
}

Touch* Actions::touch()
{
    return new Touch(m_hub);
}
