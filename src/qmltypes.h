/*
    SPDX-FileCopyrightText: 2023 Nicolas Fella <nicolas.fella@gmx.de>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include <manager.h>

#include <qqmlregistration.h>

struct NMForeign {
    Q_GADGET
    QML_ELEMENT
    QML_NAMED_ELEMENT(NetworkManager)
    QML_FOREIGN_NAMESPACE(NetworkManager)
};
